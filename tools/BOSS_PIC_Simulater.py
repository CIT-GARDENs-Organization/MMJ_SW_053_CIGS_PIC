import dataclasses
import time
import random
import re
import serial
import serial.tools.list_ports
import sys
import argparse
from datetime import datetime

setting = {
    "retransmit_time": 2,           # Retransmit limit
    "timeout": 3.0,                 # The time until retransmit
    "permission_probability": 0.9,  # Permit rate for SMF copy REQ from MIS MCU 
    "wait_time": 10,                # The time of BOSS PIC comunicating with other MIS MCU
    "debug_mode": False             # If set True, you will always be able to type command in CLI
}

# for print message decoration
class Print:
    RED     = '\033[31m'
    YELLOW  = '\033[33m'
    BLUE    = '\033[34m'
    RESET   = '\033[0m'
    BOLD    = '\033[1m'
    ERROR   = f'{RED}[ERROR]{RESET}'
    INFO    = f'{BLUE}[INFO] {RESET}'
    EVENT   = f'{YELLOW}[EVENT]{RESET}'
    LINE    = '============================='

    @staticmethod
    def get_timestamp() -> str:
        return datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    
    @staticmethod
    def timestamped(message: str) -> str:
        return f"[{Print.get_timestamp()}] {message}"

    @staticmethod
    def space_every_two_str(bytes: bytes) -> str:
        return " ".join(bytes.hex()[i:i+2].upper() for i in range(0, len(bytes.hex()), 2))
    
    @staticmethod
    def wait(sec: int) -> None:
        for _ in range(sec):
            print('.', flush=True, end='')
            time.sleep(1)
        print()


@dataclasses.dataclass
class FrameData:
    frame_id: bytes
    payload: bytes | None


# command making, check etc...
class Command:
    # most foundamental define
    SFD = b'\xAA'

    # device_id
    BOSS_PIC_DEVICE_ID = b'\x05'

    # flame id (mis mcu receive)
    STATUS_CHECK = b'\x01'
    IS_SMF_AVAILABLE = b'\x02'
    SEND_TIME = b'\x03'

    # IS_SMF_AVAILABLE payload
    ALLOW = b'\x01'
    DENY = b'\x00'

    # flame id (boss pic receive)
    MIS_MCU_STATUS = b'\x03'

    # MIS_MCU_STATUS payload
    BUSY = b'\x03'
    REQ_COPY_TO_SMF = b'\x04'
    COPYING_TO_SMF = b'\x05'
    FINISHED_MISSION = b'\x06'

    # flame id (common)
    UL_CMD = b'\x00'
    ACK = b'\x0F'

    FRAME_ID_PAYLOAD_LENGTH = {STATUS_CHECK: 0, IS_SMF_AVAILABLE: 1, SEND_TIME: 4, MIS_MCU_STATUS: 1, UL_CMD: 8, ACK: 0}
    
    @staticmethod
    def input_payload(cmd_payload=None) -> bytes:
        if cmd_payload:
            # コマンドライン引数から受け取った場合
            print(f'Using command from command line: {cmd_payload}')
            # 9バイトの16進文字列（18文字）を期待
            if re.fullmatch('[0-9A-F]{18}', cmd_payload.replace(' ', '').upper()):
                return bytes.fromhex(cmd_payload.replace(' ', '').upper())
            else:
                print(f'{Print.ERROR} Invalid command format: {cmd_payload}')
                print(f'Expected 9 bytes in hex format (18 characters)')
                # sys.exit(1)
        
        # 従来の対話的入力
        print(f'Enter uplink command in hex {Print.BOLD}(CMD ID, CMD Parameter only. {Print.RESET}SFD, Device ID, Frame ID, CRC automatically addition.)')
        print('  __ __ __ __ __ __ __ __ __')
        while True:
            input_str = input('> ').replace(' ', '').upper()
            if re.fullmatch('[0-9A-F]{18}', input_str):
                return bytes.fromhex(input_str)
            
    @staticmethod
    def calc_crc(data: bytes) -> bytes:
        crc = data[0]
        for dt in data[1:]:
            crc ^= dt
        return crc.to_bytes(1, 'big')
    
    @staticmethod
    def check_crc(data: bytes) -> bool:
        received_crc = data[-1].to_bytes(1, 'big')
        collect_crc = Command.calc_crc(data[:-1])
        if received_crc == collect_crc:
            return True
        else:
            print(Print.timestamped(f"{Print.ERROR} CRC error !"))
            print(Print.timestamped(f"\t-> received crc: {int.from_bytes(received_crc):02X}"))
            print(Print.timestamped(f"\t   collect crc : {int.from_bytes(collect_crc):02X}"))
            return False
    
    @staticmethod
    def device_id_check(data: bytes) -> bool:
        received_device_id =  ((data[1] & 0xF0) >> 4).to_bytes(1, 'big')
        if received_device_id == Command.BOSS_PIC_DEVICE_ID:
            return True
        else:
            print(Print.timestamped(f"{Print.ERROR} Device ID error. Frame ID cannot be anything other than BOSS PIC."))
            print(Print.timestamped(f"\t-> received device ID: {int.from_bytes(received_device_id):02X}"))
            print(Print.timestamped(f"\t   collect device ID : {int.from_bytes(Command.BOSS_PIC_DEVICE_ID):02X}"))
            return False

    @staticmethod
    def make_command(device_id: bytes, frame_id: bytes, payload: bytes = b'') -> bytes:
        header = ((device_id[0] << 4) | frame_id[0]).to_bytes(1, 'big')
        crc = Command.calc_crc(header + payload)
        return Command.SFD + header + payload + crc
    
    @staticmethod
    def manual_input_bytes() -> bytes:
        while True:
            bytes_str = input('> ').replace(' ', '').upper()
            bytes_len = int(len(bytes_str) / 2)
            try:
                bytes = int(bytes_str, 16).to_bytes(bytes_len, 'big')
                return bytes
            except ValueError:
                pass

    @staticmethod
    def parse_frame(data: bytes) -> FrameData:
        frame_id = (data[1] & 0x0F).to_bytes(1, 'big')
        if frame_id == Command.ACK:
            return FrameData(Command.ACK, None)
        elif frame_id == Command.UL_CMD:
            return FrameData(Command.UL_CMD, data[2:-1])
        elif frame_id == Command.MIS_MCU_STATUS:
            return FrameData(Command.MIS_MCU_STATUS, data[2].to_bytes())
        else:
            return FrameData(None, None)
    
    @staticmethod
    def pack_datetime_to_4bytes(dt: datetime | None = None) -> bytes:
        """
        現在日時を年初(1/1 00:00:00)からの経過秒として4バイト(MSB, big-endian)で返す。
        その年の1月1日 00:00:00を基準(0秒)として、現在時刻までの経過秒数を計算。
        """
        if dt is None:
            dt = datetime.now()
        
        # 年初(1月1日 00:00:00)からの経過秒を計算
        year_start = datetime(dt.year, 1, 1, 0, 0, 0)
        elapsed_seconds = int((dt - year_start).total_seconds())
        
        # 4バイト範囲チェック（最大約136年分だが、1年は約31,536,000秒なので問題なし）
        if elapsed_seconds > 0xFFFFFFFF:
            elapsed_seconds = 0xFFFFFFFF
        elif elapsed_seconds < 0:
            elapsed_seconds = 0
            
        return elapsed_seconds.to_bytes(4, 'big')
    
# role of communications in general
class Communication:
    MIS_MCU_DEVICES = {0x06: 'APRS PIC', 0x07: 'CAM MCU', 0x08: 'CHO MCU', 
                       0x0A: 'SATO PIC', 0x0B: 'BHU MCU', 0x0C: 'CIGS PIC'}

    def __init__(self):
        self.ser = None
        self.device_id = None

    def setup(self):
        self.select_port()
        self.select_device_id()

    def setup_with_args(self, com_port, baud_rate):
        """コマンドライン引数で指定されたポートとボーレートで設定"""
        try:
            print(f'Using COM port: {com_port} at {baud_rate} baud')
            self.ser = serial.Serial(com_port, baudrate=int(baud_rate), timeout=1)
            # デフォルトでCIGS PICを使用
            self.device_id = bytes.fromhex('0C')
            print(f'Using device: CIGS PIC (ID: 0C)')
        except serial.SerialException as e:
            print(f'{Print.ERROR} Failed to open {com_port}: {str(e)}')
            sys.exit(1)

    def select_port(self):
        print('Select using port')
        while True:
            ports = list(serial.tools.list_ports.comports())
            if not ports:
                input('No port found. Press any key to retry.')
                continue
            for i, port in enumerate(ports):
                print(f'{i:X}) {port.device}  ', end='\t')
            print()
            while True:
                choice_str = input('> ')
                if re.fullmatch(f'^[0-{len(ports)-1}]{{1}}$', choice_str):
                    choice = int(choice_str)
                    try:
                        self.ser: serial.Serial = serial.Serial(ports[choice].device, baudrate=9600, timeout=1)
                    except serial.SerialException as e:
                        print(str(e))
                        continue
                    return

    def select_device_id(self):
        while True:
            print('\nSelect your device:')
            for id, name in self.MIS_MCU_DEVICES.items():
                print(f'{id:X}) {name}', end='\t')
            print()

            choice = input('> ').strip().upper()
            if re.fullmatch(f'^[6-9A-F]$', choice):
                self.device_id = bytes.fromhex('0' + choice)
                print(f'Using device: {self.MIS_MCU_DEVICES[int(choice, 16)]}')
                return

    def transmit_and_receive_command(self, command: bytes) -> bytes | None:
        retransmission_time = setting["retransmit_time"]
        for _ in range(retransmission_time + 1):
            print(Print.timestamped(f'{Print.EVENT} BOSS PIC transmit command'))
            print(Print.timestamped(f'\t\t{Print.BOLD}BOSS > > > [{Print.space_every_two_str(command)}] > > > MIS MCU{Print.RESET}\n'))
            self.ser.write(command)
            if not setting["debug_mode"]:
                response = self.receive()
            else:
                response = Command.manual_input_bytes() # for debug
            if not response:
                continue
            if len(response) > 2:
                if Command.device_id_check(response):
                    if Command.check_crc(response[1:]):
                        print(Print.timestamped(f'{Print.EVENT} BOSS PIC received command'))
                        print(Print.timestamped(f'\t\t{Print.BOLD}BOSS < < < [{Print.space_every_two_str(response)}] < < < MIS MCU\n'))
                        return response
        return None
    
    def receive(self) -> bytes | None:    # TODO: write more smater code.
        response = b''
        timeout = setting["timeout"]
        start_time = time.time()
        while time.time() - start_time < timeout:
            if self.ser.in_waiting > 0:
                response += self.ser.read(1)
                if response == Command.SFD:
                    time.sleep(0.5) # wait for receive all data
                    response += self.ser.read_all()
                    return response
                else:
                    response = b''
            time.sleep(0.1) # rest for cpu
        return None

    def respond_to_req(self) -> None:
        if random.random() < setting["permission_probability"]:
            print(Print.timestamped(f"\t  -> BOSS PIC allow copying data to SMF"))
            allow_command = Command.make_command(self.device_id, Command.IS_SMF_AVAILABLE, Command.ALLOW)
            response = self.transmit_and_receive_command(allow_command)
        else:
            print(Print.timestamped(f"\t  -> BOSS PIC deny copying data to SMF"))
            deny_command = Command.make_command(self.device_id, Command.IS_SMF_AVAILABLE, Command.DENY)
            response = self.transmit_and_receive_command(deny_command)

        frame_data = Command.parse_frame(response)
        if not frame_data.frame_id:
            quit_software(self, 3)
        elif frame_data.frame_id == Command.ACK:
            print(Print.timestamped(f"{Print.INFO} BOSS PIC receive ACK frame"))
        else:
            quit_software(self, 4)

    def close(self):
        if self.ser:
            self.ser.close()

def quit_software(com: Communication, code: int) -> None:
    if code == 0:
        print(Print.timestamped(f""))
    if code == 1:
        print(Print.timestamped(f'{Print.ERROR} BOSS PIC didn\'t receive command.'))
        print(Print.timestamped(f'\t->BOSS PIC assumed error has occured in MIS MCU'))
        print(Print.timestamped(f'\t  MIS MCU is stopped by BOSS PIC'))
    elif code == 2:
        print(Print.timestamped(f'{Print.ERROR} BOSS PIC didn\'t receive ACK')) 
    elif code == 3:
        print(Print.timestamped(f'{Print.ERROR} Frame ID doesn\'t exist'))
    elif code == 4:
        print(Print.timestamped(f'{Print.ERROR} It\'s impossible to BOSS PIC receive ACK in this case'))

    close_and_exit(com)

def close_and_exit(com) -> None:
    com.close()
    print(Print.timestamped(f"Software quit."))
    sys.exit()


def send_current_time(com: Communication) -> bool:
    """現在時刻(MM/DD hh:mm:ss)を4バイトに詰めてSEND_TIMEで送信する。ACKを期待。"""
    payload = Command.pack_datetime_to_4bytes()
    assert com.device_id is not None
    device_id: bytes = com.device_id  # type: ignore[assignment]
    cmd = Command.make_command(device_id, Command.SEND_TIME, payload)
    print(Print.timestamped(f"{Print.INFO} SEND_TIME を送信 (payload={Print.space_every_two_str(payload)})"))
    resp = com.transmit_and_receive_command(cmd)
    if not resp:
        print(Print.timestamped(f"{Print.ERROR} SEND_TIME の応答がありません"))
        return False
    frame = Command.parse_frame(resp)
    if frame.frame_id == Command.ACK:
        print(Print.timestamped(f"{Print.INFO} SEND_TIME の ACK を受信"))
        return True
    print(Print.timestamped(f"{Print.ERROR} SEND_TIME 後の応答がACKではありません"))
    return False


def main(com_port=None, baud_rate=None, command=None, send_time_with_command=False):
    print(f'\n================================')
    print(f'=== {Print.BOLD}BOSS PIC Simulator v1.00{Print.RESET} ===')
    print(f'================================\n')

    com = Communication()
    if com_port and baud_rate:
        # COMポートとボーレートが指定された場合
        com.setup_with_args(com_port, baud_rate)
    else:
        # 従来の対話的設定
        com.setup()

    print(f'\n{Print.LINE}\n')
    
    # コマンド引数が指定されている場合は単発実行
    if command:
        # -c と -t の組み合わせで時刻送信を選択可能
        if send_time_with_command:
            if not send_current_time(com):
                print(Print.timestamped(f"{Print.ERROR} 時刻送信に失敗しました"))
        # UL_CMD を送信
        uplink_command_payload = Command.input_payload(command)
        assert com.device_id is not None
        device_id: bytes = com.device_id  # type: ignore[assignment]
        uplink_command = Command.make_command(device_id, Command.UL_CMD, uplink_command_payload)
        execute_mission(com, uplink_command)
    else:
        # インタラクティブモード
        interactive_mode(com)

def interactive_mode(com):
    """インタラクティブモードでコマンドを連続実行"""
    print(f'{Print.INFO} インタラクティブモードで開始しました')
    print(f'コマンド入力待機中... (\'exit\' で終了)')
    print(f'使用方法:')
    print(f'  - 16進数でコマンドを入力: C0 00 00 00 00 00 00 00 00')
    print(f'  - コマンド末尾に " -t" を付けると時刻送信後にコマンド送信')
    print(f'  - "time" と入力で現在の MM/DD hh:mm:ss を4バイト(SEND_TIME)で送信')
    print(f'  - command_uiからコマンドを送信可能')
    print(f'  - \'exit\' または Ctrl+C で終了')
    print(f'{Print.LINE}')
    
    try:
        while True:
            try:
                print(f'\n> ', end='', flush=True)
                user_input = input().strip()
                
                if user_input.lower() in ['exit', 'quit', 'q']:
                    break
                
                if not user_input:
                    continue
                
                # ショートカット: 現在時刻を送信
                if user_input.lower() in ['time', 't']:
                    payload = Command.pack_datetime_to_4bytes()
                    assert com.device_id is not None
                    device_id: bytes = com.device_id  # type: ignore[assignment]
                    cmd = Command.make_command(device_id, Command.SEND_TIME, payload)
                    print(Print.timestamped(f"{Print.INFO} SEND_TIME を送信 (payload={Print.space_every_two_str(payload)})"))
                    resp = com.transmit_and_receive_command(cmd)
                    if not resp:
                        print(Print.timestamped(f'{Print.ERROR} 応答がありません - 中断'))
                        continue
                    frame = Command.parse_frame(resp)
                    if frame.frame_id == Command.ACK:
                        print(Print.timestamped(f"{Print.INFO} ACK 受信"))
                    else:
                        print(Print.timestamped(f"{Print.ERROR} 予期しない応答"))
                    continue
                
                # コマンド末尾に -t があるかチェック
                send_time_first = False
                command_input = user_input
                if user_input.strip().endswith(' -t'):
                    send_time_first = True
                    command_input = user_input.strip()[:-3].strip()  # -t を除去
                
                # 時刻送信（-t指定時のみ）
                if send_time_first:
                    if not send_current_time(com):
                        print(Print.timestamped(f"{Print.ERROR} 時刻送信に失敗しました"))
                
                # UL_CMD を送信
                uplink_command_payload = Command.input_payload(command_input)
                assert com.device_id is not None
                device_id: bytes = com.device_id  # type: ignore[assignment]
                uplink_command = Command.make_command(device_id, Command.UL_CMD, uplink_command_payload)
                
                # ミッション実行
                print(f'{Print.INFO} コマンド実行開始...')
                execute_mission(com, uplink_command)
                print(f'{Print.INFO} コマンド実行完了')
                
            except KeyboardInterrupt:
                print(f'\n{Print.INFO} ユーザーによって中断されました')
                break
            except Exception as e:
                print(f'{Print.ERROR} エラーが発生しました: {str(e)}')
                continue
                
    finally:
        print(f'\n{Print.INFO} インタラクティブモードを終了します')
        com.close()

def execute_mission(com, uplink_command):
    """ミッションを実行"""
    print(Print.timestamped(f"{Print.INFO} BOSS PIC received uplink command"))
    time.sleep(1)

    response = com.transmit_and_receive_command(uplink_command)
    
    while True: # BOSS PIC start continuous communication whith MIS MCU
        if not response:
            print(Print.timestamped(f'{Print.ERROR} 応答がありません - ミッション中断'))
            return False

        frame_data = Command.parse_frame(response)
        if frame_data.frame_id == Command.ACK:
            print(Print.timestamped(f"{Print.INFO} BOSS PIC receive ACK frame"))

        elif frame_data.frame_id == Command.MIS_MCU_STATUS:
            print(Print.timestamped(f"{Print.INFO} BOSS PIC receive MIS MCU Status frame"))
            if not frame_data.frame_id:
                print(Print.timestamped(f'{Print.ERROR} ACKを受信できませんでした'))
                return False

            if frame_data.payload == Command.BUSY:
                print(Print.timestamped(f"\t-> Executing mission"))

            elif frame_data.payload == Command.REQ_COPY_TO_SMF:
                print(Print.timestamped(f"\t-> Request copy to SMF"))
                com.respond_to_req()

            elif frame_data.payload == Command.COPYING_TO_SMF:
                print(Print.timestamped(f"\t-> Copying data to SMF"))

            elif frame_data.payload == Command.FINISHED_MISSION:
                print(Print.timestamped(f"\t-> Finished mission"))
                print(Print.timestamped(f"\t -> BOSS PIC power off MIS MCU "))
                return True
        else:
            print(Print.timestamped(f'{Print.ERROR} 不明なフレームIDです'))
            return False

        time.sleep(1)
        print(Print.timestamped(f"{Print.INFO} BOSS PIC switch CPLD rooting"))
        print(Print.timestamped(f'\t-> Communicating other MIS MCU'), end='')
        Print.wait(setting["wait_time"])
        print(Print.timestamped(f"{Print.INFO} BOSS PIC switch CPLD rooting"))
        print(Print.timestamped(f'\t-> Connection is to you'))
        time.sleep(1)
        status_check_command = Command.make_command(com.device_id, Command.STATUS_CHECK, b'')
        response = com.transmit_and_receive_command(status_check_command)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='BOSS PIC Simulator')
    parser.add_argument('--port', '-p', help='COM port (e.g., COM3)')
    parser.add_argument('--baud', '-b', help='Baud rate (default: 9600)', default='9600')
    parser.add_argument('--command', '-c', help='Command to send (9 bytes in hex, e.g., "01 00 00 00 00 00 00 00 00")')
    parser.add_argument('--send-time', '-t', action='store_true', help='Send current time before command (use with -c) or standalone')
    
    args = parser.parse_args()
    
    # --send-time のみの場合は時刻送信して終了
    if args.send_time and not args.command:
        com = Communication()
        if args.port and args.baud:
            com.setup_with_args(args.port, args.baud)
        else:
            com.setup()
        payload = Command.pack_datetime_to_4bytes()
        assert com.device_id is not None
        device_id: bytes = com.device_id  # type: ignore[assignment]
        cmd = Command.make_command(device_id, Command.SEND_TIME, payload)
        print(Print.timestamped(f"{Print.INFO} SEND_TIME を送信 (payload={Print.space_every_two_str(payload)})"))
        resp = com.transmit_and_receive_command(cmd)
        if not resp:
            print(Print.timestamped(f'{Print.ERROR} 応答がありません - 終了'))
        else:
            frame = Command.parse_frame(resp)
            if frame.frame_id == Command.ACK:
                print(Print.timestamped(f"{Print.INFO} ACK 受信"))
            else:
                print(Print.timestamped(f"{Print.ERROR} 予期しない応答"))
        com.close()
        sys.exit(0)

    main(args.port, args.baud, args.command, args.send_time)
