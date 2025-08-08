#include "mmj_cigs_flash.h"                           // 同じフォルダのヘッダー
#include "../../system/mmj_cigs_config.h"             // システム設定
#include "../../../lib/device/mt25q.h"                // デバイスライブラリ
#include "../../../lib/tool/calc_tools.h"             // ツールライブラリ
#include "../../../lib/tool/smf_queue.h"              // SMFキュー管理
void misf_init()
{
    fprintf(PC, "MISSION FLASH Initialize\r\n");

    // 初期状態としてCSを非アクティブに
    output_high(MIS_FM_CS);
    output_high(SMF_CS);
    delay_ms(100);

    //=== [MIS_FM] READ ID ===//
    READ_ID_DATA misf_read_id_data;
    int8 flash_cmd = CMD_READ_ID;

    output_low(mis_fm.cs_pin);
    spi_xfer_and_read_select_stream(mis_fm, &flash_cmd, 1, misf_read_id_data.bytes, READ_ID_DATASIZE);
    output_high(mis_fm.cs_pin);

    fprintf(PC, "\t[MIS FM] READ ID: ");
    for (unsigned int8 i = 0; i < 4; i++) {
        fprintf(PC, "%02X ", misf_read_id_data.bytes[i]);
    }
    fprintf(PC, "\r\n");

    //=== [SMF] READ ID ===//
    READ_ID_DATA smf_read_id_data;

    output_low(SMF_CS);
    delay_us(100); // 微小ディレイ
    spi_xfer_and_read_select_stream(smf, &flash_cmd, 1, smf_read_id_data.bytes, READ_ID_DATASIZE);
    output_high(SMF_CS);

    fprintf(PC, "\t[SMF   ] READ ID: ");
    for (unsigned int8 i = 0; i < 4; i++) {
        fprintf(PC, "%02X ", smf_read_id_data.bytes[i]);
    }
    fprintf(PC, "\r\n");

    //=== 接続確認 ===//
    if (!is_connect(smf)) {
        fprintf(PC, "\t[SMF   ] connect error!\r\n");
    } else {
        fprintf(PC, "\t[SMF   ] is connected\r\n");
    }

    delay_ms(100);

    if (!is_connect(mis_fm)) {
        fprintf(PC, "\t[MIS FM] connect error!\r\n");
    } else {
        fprintf(PC, "\t[MIS FM] is connected\r\n");
    }

    //=== MIS_FMからログ領域読み出し ===//
    FlashData_t flash_data;
    read_misf_address_area(flash_data.bytes);

    if (flash_data.packet.crc != calc_crc8(flash_data.bytes, PACKET_SIZE - 1)) {
        fprintf(PC, "\t[MIS FM] CRC error!\r\n");
        return;
    }

    //=== カウンタ情報の反映 ===//
    piclog_data       = *((Flash_t*)&flash_data.packet.payload.logdata.piclog);
    environment_data  = *((Flash_t*)&flash_data.packet.payload.logdata.environment);
    iv_header         = *((Flash_t*)&flash_data.packet.payload.logdata.iv_header);
    iv_data           = *((Flash_t*)&flash_data.packet.payload.logdata.iv_data);
}

void smf_init()
{
   //Flash smf = {SPI_1, MT25QL01GBBB, SPI1_CS};
}





void read_misf_address_area(unsigned int8 *data)
{
    read_data_bytes(mis_fm, ADDRESS_MANAGE_START, data, PACKET_SIZE);
}

void write_misf_address_area()
{
    FlashData_t flash_data;
    flash_data.packet.crc = calc_crc8(flash_data.bytes, PACKET_SIZE - 1);
    flash_data.packet.payload.logdata.piclog.used_counter = piclog_data.used_counter;
    flash_data.packet.payload.logdata.piclog.uncopied_counter = piclog_data.uncopied_counter;
    flash_data.packet.payload.logdata.piclog.reserve_counter1 = piclog_data.reserve_counter1;
    flash_data.packet.payload.logdata.piclog.reserve_counter2 = piclog_data.reserve_counter2;
    flash_data.packet.payload.logdata.environment.used_counter = environment_data.used_counter;
    flash_data.packet.payload.logdata.environment.uncopied_counter = environment_data.uncopied_counter;
    flash_data.packet.payload.logdata.environment.reserve_counter1 = environment_data.reserve_counter1;
    flash_data.packet.payload.logdata.environment.reserve_counter2 = environment_data.reserve_counter2;
    flash_data.packet.payload.logdata.iv_header.used_counter = iv_header.used_counter;
    flash_data.packet.payload.logdata.iv_header.uncopied_counter = iv_header.uncopied_counter;
    flash_data.packet.payload.logdata.iv_header.reserve_counter1 = iv_header.reserve_counter1;
    flash_data.packet.payload.logdata.iv_header.reserve_counter2 = iv_header.reserve_counter2;
    flash_data.packet.payload.logdata.iv_data.used_counter = iv_data.used_counter;
    flash_data.packet.payload.logdata.iv_data.uncopied_counter = iv_data.uncopied_counter;
    flash_data.packet.payload.logdata.iv_data.reserve_counter1 = iv_data.reserve_counter1;
    flash_data.packet.payload.logdata.iv_data.reserve_counter2 = iv_data.reserve_counter2;

    write_data_bytes(mis_fm, ADDRESS_MANAGE_START, flash_data.bytes, PACKET_SIZE);
}

void add_smf_queue(int8 mission_id)
{
    FlashOperationStruct data;
    SmfMissionStruct mis_struct;
    
    // 構造体の初期化
    data.func_type = SMF_WRITE;
    data.mission_id = mission_id;
    data.write_mode = SMF_WRITE_CIRCULAR;
    data.source_type = SOURCE_MISF_UNCOPIED;
    data.src = 0;
    data.size = 0;
    data.misf_start_addr = 0;
    data.misf_size = 0;
    data.manager = 0;
    
    //mis_struct = get_smf_mission_struct(SMF_WRITE);

    enqueue_flash_operation(&data);
}

void print_flash_status()
{
    fprintf(PC, "\t| MISF | PICLOG    | Use Counter      : 0x%08LX\r\n", piclog_data.used_counter);
    fprintf(PC, "\t| MISF | PICLOG    | Uncopyed Counter : 0x%08LX\r\n", piclog_data.uncopied_counter);
    fprintf(PC, "\t| MISF | PICLOG    | Reserve Counter1 : 0x%02X\r\n", piclog_data.reserve_counter1);
    fprintf(PC, "\t| MISF | PICLOG    | Reserve Counter2 : 0x%02X\r\n", piclog_data.reserve_counter2);
    fprintf(PC, "\t| MISF | ENVIRO    | Use Counter      : 0x%08LX\r\n", environment_data.used_counter);
    fprintf(PC, "\t| MISF | ENVIRO    | Uncopyed Counter : 0x%08LX\r\n", environment_data.uncopied_counter);
    fprintf(PC, "\t| MISF | ENVIRO    | Reserve Counter1 : 0x%02X\r\n", environment_data.reserve_counter1);
    fprintf(PC, "\t| MISF | ENVIRO    | Reserve Counter2 : 0x%02X\r\n", environment_data.reserve_counter2);
    fprintf(PC, "\t| MISF | IV HEADER | Use Counter      : 0x%08LX\r\n", iv_header.used_counter);
    fprintf(PC, "\t| MISF | IV HEADER | Uncopyed Counter : 0x%08LX\r\n", iv_header.uncopied_counter);
    fprintf(PC, "\t| MISF | IV HEADER | Reserve Counter1 : 0x%02X\r\n", iv_header.reserve_counter1);
    fprintf(PC, "\t| MISF | IV HEADER | Reserve Counter2 : 0x%02X\r\n", iv_header.reserve_counter2);
    fprintf(PC, "\t| MISF | IV DATA   | Use Counter      : 0x%08LX\r\n", iv_data.used_counter);
    fprintf(PC, "\t| MISF | IV DATA   | Uncopyed Counter : 0x%08LX\r\n", iv_data.uncopied_counter);
    fprintf(PC, "\t| MISF | IV DATA   | Reserve Counter1 : 0x%02X\r\n", iv_data.reserve_counter1);
    fprintf(PC, "\t| MISF | IV DATA   | Reserve Counter2 : 0x%02X\r\n", iv_data.reserve_counter2);


    fprintf(PC, "\tComplete\r\n");
}
