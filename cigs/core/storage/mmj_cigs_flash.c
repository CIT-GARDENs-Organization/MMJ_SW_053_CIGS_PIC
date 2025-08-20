#include "mmj_cigs_flash.h"                           // 同じフォルダのヘッダー
#include "../../system/mmj_cigs_config.h"             // システム設定
#include "../../../lib/device/mt25q.h"                // デバイスライブラリ
#include "../../../lib/tool/calc_tools.h"             // ツールライブラリ
#include "../../../lib/tool/smf_queue.h"              // SMFキュー管理
void misf_init()
{
    fprintf(PC, "MISSION FLASH Initialize\r\n");
    output_high(MIS_FM_CS);
    output_high(SMF_CS);
    delay_ms(100);

    // 読み込みID処理 ・・・(既存そのまま)
    if (is_connect(mis_fm)) {
        fprintf(PC, "\t[MIS FM] Connected\r\n");
    } else {
        fprintf(PC, "\t[MIS FM] Not Connected\r\n");
        return;
    }

    if (is_connect(smf)) {
        fprintf(PC, "\t[SMF] Connected\r\n");
    } else {
        fprintf(PC, "\t[SMF] Not Connected\r\n");
        return;
    }

    //=== MIS_FM カウンタテーブル読出し ===//
    FlashData_t flash_data;
    memset(flash_data.bytes, 0, PACKET_SIZE);
    read_data_bytes(mis_fm, MISF_CIGS_DATA_TABLE_START, flash_data.bytes, PACKET_SIZE);

    // CRC 検証
    if (flash_data.packet.crc != calc_crc8(flash_data.bytes, PACKET_SIZE - 1)) {
        fprintf(PC, "\t[MIS FM] CRC error -> initialize counters\r\n");
        // デフォルト初期化
        /*
        piclog_data.id        = FLASH_ID_PICLOG;
        environment_data.id   = FLASH_ID_ENVIRONMENT;
        iv_header.id          = FLASH_ID_IV_HEADER;
        iv_data.id            = FLASH_ID_IV_DATA;
        piclog_data.used_counter = 0;
        piclog_data.uncopied_counter = 0;
        environment_data.used_counter = 0;
        environment_data.uncopied_counter = 0;
        iv_header.used_counter = 0;
        iv_header.uncopied_counter = 0;
        iv_data.used_counter = 0;
        iv_data.uncopied_counter = 0;
        write_misf_address_area();   // 初期テーブル書込み
        print_flash_status();
        return;
        */
    }

    //=== 個別コピー (キャスト禁止) ===//
    piclog_data.id        = FLASH_ID_PICLOG;
    piclog_data.used_counter     = flash_data.packet.payload.logdata.piclog.used_counter;
    piclog_data.uncopied_counter = flash_data.packet.payload.logdata.piclog.uncopied_counter;
    piclog_data.reserve_counter1 = flash_data.packet.payload.logdata.piclog.reserve_counter1;
    piclog_data.reserve_counter2 = flash_data.packet.payload.logdata.piclog.reserve_counter2;

    environment_data.id        = FLASH_ID_ENVIRONMENT;
    environment_data.used_counter     = flash_data.packet.payload.logdata.environment.used_counter;
    environment_data.uncopied_counter = flash_data.packet.payload.logdata.environment.uncopied_counter;
    environment_data.reserve_counter1 = flash_data.packet.payload.logdata.environment.reserve_counter1;
    environment_data.reserve_counter2 = flash_data.packet.payload.logdata.environment.reserve_counter2;

    iv_header.id        = FLASH_ID_IV_HEADER;
    iv_header.used_counter     = flash_data.packet.payload.logdata.iv_header.used_counter;
    iv_header.uncopied_counter = flash_data.packet.payload.logdata.iv_header.uncopied_counter;
    iv_header.reserve_counter1 = flash_data.packet.payload.logdata.iv_header.reserve_counter1;
    iv_header.reserve_counter2 = flash_data.packet.payload.logdata.iv_header.reserve_counter2;

    iv_data.id        = FLASH_ID_IV_DATA;
    iv_data.used_counter     = flash_data.packet.payload.logdata.iv_data.used_counter;
    iv_data.uncopied_counter = flash_data.packet.payload.logdata.iv_data.uncopied_counter;
    iv_data.reserve_counter1 = flash_data.packet.payload.logdata.iv_data.reserve_counter1;
    iv_data.reserve_counter2 = flash_data.packet.payload.logdata.iv_data.reserve_counter2;

    print_flash_status();
}

FlashData_t make_flash_data_table()
{
    FlashData_t flash_data;
    memset(&flash_data.bytes, 0, sizeof(flash_data.bytes));

    FlashCounter_t *dst_list[] = {
        &flash_data.packet.payload.logdata.piclog,
        &flash_data.packet.payload.logdata.environment,
        &flash_data.packet.payload.logdata.iv_header,
        &flash_data.packet.payload.logdata.iv_data
    };
    Flash_t *src_list[] = {
        &piclog_data,
        &environment_data,
        &iv_header,
        &iv_data
    };

    for (int i = 0; i < 4; i++) {
        dst_list[i]->used_counter     = src_list[i]->used_counter;
        dst_list[i]->uncopied_counter = src_list[i]->uncopied_counter;
        dst_list[i]->reserve_counter1 = src_list[i]->reserve_counter1;
        dst_list[i]->reserve_counter2 = src_list[i]->reserve_counter2;
    }

    flash_data.packet.crc = calc_crc8(flash_data.bytes, PACKET_SIZE - 1);
    // fprintf(PC, "make table\r\n");
    // for (int i = 0; i < PACKET_SIZE; i++) {
        // fprintf(PC, "%02X ", flash_data.bytes[i]);
    // }
    fprintf(PC, "\r\n");
    return flash_data;
}


void write_misf_address_area(void)
{
    FlashData_t flash_data = make_flash_data_table();
    sector_erase(mis_fm, MISF_CIGS_DATA_TABLE_START);
    // START に書く (以前 END だった)
    write_data_bytes(mis_fm, MISF_CIGS_DATA_TABLE_START, flash_data.bytes, PACKET_SIZE);
}

// PICLOG 更新後に呼ばれる関数例 (piclog_write 内で既に write_misf_address_area 呼んでいる場合は不要)
static void update_piclog_counters_after_write(unsigned int16 wrote_size)
{
    piclog_data.used_counter     += wrote_size;
    piclog_data.uncopied_counter += wrote_size;
    // 必要なら wrap 処理追加
    write_misf_address_area();
}

void add_smf_queue(MissionID mission_id, FunctionType func_type, SmfWriteMode write_mode)
{
    FlashOperationStruct data;
    // 構造体の初期化
    data.mission_id = mission_id;
    data.func_type = func_type;
    data.write_mode = write_mode;
    data.source_type = SOURCE_MISF_UNCOPIED;
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

void write_smf_header(smf_data_table_t *smf_data_table)
{

    int8 readdata[PACKET_SIZE];
    int8 retry_count;
    int1 crc_valid = 0;
    for (retry_count = 0; retry_count < CRC_RETRY_COUNT; retry_count++)
    {
        subsector_4kByte_erase(smf, CIGS_DATA_TABLE_START_ADDRESS);
        write_data_bytes(smf, CIGS_DATA_TABLE_START_ADDRESS, smf_data_table->bytes, PACKET_SIZE); // ヘッダーを書き込み
        read_data_bytes(smf, CIGS_DATA_TABLE_START_ADDRESS, readdata, PACKET_SIZE);
        if (is_crc_valid(readdata, PACKET_SIZE-1))
        {
            fprintf(PC, "CRC verification passed on attempt %d\r\n", retry_count + 1);
            break;
        }
    }
    if (!crc_valid)
    {
        fprintf(PC, "Error: CRC verification failed after %d attempts\r\n", CRC_RETRY_COUNT);
        // return; 
    }
}

MisfAddressStruct get_misf_address_struct(MissionID mission_id)
{
   MisfAddressStruct mis_struct = {0};

   if (mission_id == CIGS_DATA_TABLE)
   {
      mis_struct.start_address = MISF_CIGS_DATA_TABLE_START;
      mis_struct.end_address   = MISF_CIGS_DATA_TABLE_END;
   }
   else if (mission_id == CIGS_PICLOG_DATA)
   {
      mis_struct.start_address = MISF_CIGS_PICLOG_START;
      mis_struct.end_address   = MISF_CIGS_PICLOG_END;
   }
   else if (mission_id == CIGS_ENVIRO_DATA)
   {
      mis_struct.start_address = MISF_CIGS_ENVIRO_START;
      mis_struct.end_address   = MISF_CIGS_ENVIRO_END;
   }
   else if (mission_id == CIGS_IV_HEADER)
   {
      mis_struct.start_address = MISF_CIGS_IV_HEADER_START;
      mis_struct.end_address   = MISF_CIGS_IV_HEADER_END;
   }
   else if (mission_id == CIGS_IV_DATA)
   {
      mis_struct.start_address = MISF_CIGS_IV_DATA_START;
      mis_struct.end_address   = MISF_CIGS_IV_DATA_END;
   }
   return mis_struct;
}

MisfWriteStruct get_misf_write_struct(MissionID mission_id)
{
    MisfWriteStruct mis_write_struct = {0};

    if (mission_id == CIGS_DATA_TABLE)
    {
        mis_write_struct.start_address = MISF_CIGS_DATA_TABLE_START;
        mis_write_struct.size = MISF_CIGS_DATA_TABLE_SIZE;
    }
    else if (mission_id == CIGS_PICLOG_DATA)
    {
        mis_write_struct.start_address = MISF_CIGS_PICLOG_START + piclog_data.used_counter - piclog_data.uncopied_counter;
        mis_write_struct.size = piclog_data.uncopied_counter;
    }
    else if (mission_id == CIGS_ENVIRO_DATA)
    {
        mis_write_struct.start_address = MISF_CIGS_ENVIRO_START + environment_data.used_counter - environment_data.uncopied_counter;
        mis_write_struct.size = environment_data.uncopied_counter;
    }
    else if (mission_id == CIGS_IV_HEADER)
    {
        mis_write_struct.start_address = MISF_CIGS_IV_HEADER_START + iv_header.used_counter - iv_header.uncopied_counter;
        mis_write_struct.size = iv_header.uncopied_counter;
    }
    else if (mission_id == CIGS_IV_DATA)
    {
        mis_write_struct.start_address = MISF_CIGS_IV_DATA_START + iv_data.used_counter - iv_data.uncopied_counter;
        mis_write_struct.size = iv_data.uncopied_counter;
    }
    return mis_write_struct;
}
// End of file