#include "mmj_cigs_flash.h"                           
#include "../hal/mmj_cigs_config.h"             
#include "../lib/device/mt25q.h"                
#include "../lib/tool/calc_tools.h"             
#include "../lib/tool/smf_queue.h"              

// 実体宣言
Flash_t data_table = {FLASH_ID_DATA_TABLE, 0, 0, 0};
Flash_t piclog_data = {FLASH_ID_PICLOG, 0, 0, 0};
Flash_t environment_data = {FLASH_ID_ENVIRONMENT, 0, 0, 0};
Flash_t iv1_header = {FLASH_ID_IV1_HEADER, 0, 0, 0};
Flash_t iv1_data = {FLASH_ID_IV1_DATA, 0, 0, 0};
Flash_t iv2_header = {FLASH_ID_IV2_HEADER, 0, 0, 0};
Flash_t iv2_data = {FLASH_ID_IV2_DATA, 0, 0, 0};

// ポインタ設定
Flash_t *data_table_ptr = &data_table;
Flash_t *piclog_data_ptr = &piclog_data;
Flash_t *environment_data_ptr = &environment_data;
Flash_t *iv1_header_ptr = &iv1_header;
Flash_t *iv1_data_ptr = &iv1_data;
Flash_t *iv2_header_ptr = &iv2_header;
Flash_t *iv2_data_ptr = &iv2_data;

void misf_init()
{
    fprintf(PC, "MISSION FLASH Initialize\r\n");
    output_high(MIS_FM_CS);
    output_high(SMF_CS);

    // 接続処理
    if (is_connect(mis_fm)) {
        fprintf(PC, "\t[MIS FM] Connected\r\n");
    } else {
        fprintf(PC, "\t[MIS FM] Not Connected\r\n");
    }
    if (is_connect(smf)) {
        fprintf(PC, "\t[SMF] Connected\r\n");
    } else {
        fprintf(PC, "\t[SMF] Not Connected\r\n");
    }

    //=== MIS_FM カウンタテーブル読出し ===//
    FlashData_t read_header;
    FlashData_t *read_header_ptr = &read_header;
    memset(read_header_ptr, 0x00, sizeof(FlashData_t));
    read_data_bytes(mis_fm, MISF_CIGS_DATA_TABLE_START, read_header_ptr->bytes, PACKET_SIZE);

    // CRC 検証
    if (read_header_ptr->packet.crc != calc_crc8(read_header_ptr->bytes, PACKET_SIZE - 1)) {
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
    // PICLOG
    piclog_data_ptr->used_counter     = read_header_ptr->packet.payload.logdata.piclog.used_counter;
    piclog_data_ptr->uncopied_counter = read_header_ptr->packet.payload.logdata.piclog.uncopied_counter;
    piclog_data_ptr->reserve_counter1 = read_header_ptr->packet.payload.logdata.piclog.reserve_counter1;
    piclog_data_ptr->reserve_counter2 = read_header_ptr->packet.payload.logdata.piclog.reserve_counter2;
    // 環境データ
    environment_data_ptr->used_counter     = read_header_ptr->packet.payload.logdata.environment.used_counter;
    environment_data_ptr->uncopied_counter = read_header_ptr->packet.payload.logdata.environment.uncopied_counter;
    environment_data_ptr->reserve_counter1 = read_header_ptr->packet.payload.logdata.environment.reserve_counter1;
    environment_data_ptr->reserve_counter2 = read_header_ptr->packet.payload.logdata.environment.reserve_counter2;
    // IV1 ヘッダ
    iv1_header_ptr->used_counter     = read_header_ptr->packet.payload.logdata.iv1_header.used_counter;
    iv1_header_ptr->uncopied_counter = read_header_ptr->packet.payload.logdata.iv1_header.uncopied_counter;
    iv1_header_ptr->reserve_counter1 = read_header_ptr->packet.payload.logdata.iv1_header.reserve_counter1;
    iv1_header_ptr->reserve_counter2 = read_header_ptr->packet.payload.logdata.iv1_header.reserve_counter2;
    // IV1 データ
    iv1_data_ptr->used_counter     = read_header_ptr->packet.payload.logdata.iv1_data.used_counter;
    iv1_data_ptr->uncopied_counter = read_header_ptr->packet.payload.logdata.iv1_data.uncopied_counter;
    iv1_data_ptr->reserve_counter1 = read_header_ptr->packet.payload.logdata.iv1_data.reserve_counter1;
    iv1_data_ptr->reserve_counter2 = read_header_ptr->packet.payload.logdata.iv1_data.reserve_counter2;
    // IV2 ヘッダ
    iv2_header_ptr->used_counter     = read_header_ptr->packet.payload.logdata.iv2_header.used_counter;
    iv2_header_ptr->uncopied_counter = read_header_ptr->packet.payload.logdata.iv2_header.uncopied_counter;
    iv2_header_ptr->reserve_counter1 = read_header_ptr->packet.payload.logdata.iv2_header.reserve_counter1;
    iv2_header_ptr->reserve_counter2 = read_header_ptr->packet.payload.logdata.iv2_header.reserve_counter2;
    // IV2 データ
    iv2_data_ptr->used_counter     = read_header_ptr->packet.payload.logdata.iv2_data.used_counter;
    iv2_data_ptr->uncopied_counter = read_header_ptr->packet.payload.logdata.iv2_data.uncopied_counter;
    iv2_data_ptr->reserve_counter1 = read_header_ptr->packet.payload.logdata.iv2_data.reserve_counter1;
    iv2_data_ptr->reserve_counter2 = read_header_ptr->packet.payload.logdata.iv2_data.reserve_counter2;
    
    print_flash_status();
}

FlashData_t make_flash_data_table()
{
    FlashData_t flash_data;
    FlashData_t *flash_data_ptr = &flash_data;
    memset(flash_data_ptr->bytes, 0, sizeof(flash_data_ptr->bytes));

    FlashCounter_t *dst_list[] = {
        &flash_data.packet.payload.logdata.piclog,
        &flash_data.packet.payload.logdata.environment,
        &flash_data.packet.payload.logdata.iv1_header,
        &flash_data.packet.payload.logdata.iv1_data,
        &flash_data.packet.payload.logdata.iv2_header,
        &flash_data.packet.payload.logdata.iv2_data
    };
    Flash_t *src_list[] = {
        &piclog_data,
        &environment_data,
        &iv1_header,
        &iv1_data,
        &iv2_header,
        &iv2_data
    };

    for (int i = 0; i < FLASH_ID_COUNT; i++) {
        dst_list[i]->used_counter     = src_list[i]->used_counter;
        dst_list[i]->uncopied_counter = src_list[i]->uncopied_counter;
        dst_list[i]->reserve_counter1 = src_list[i]->reserve_counter1;
        dst_list[i]->reserve_counter2 = src_list[i]->reserve_counter2;
    }

    flash_data.packet.crc = calc_crc8(flash_data.bytes, PACKET_SIZE - 1);
    fprintf(PC, "\r\n");
    return flash_data;
}


void write_misf_address_area(void)
{
    FlashData_t flash_data = make_flash_data_table();
    FlashData_t *flash_data_ptr = &flash_data;
    sector_erase(mis_fm, MISF_CIGS_DATA_TABLE_START);
    write_data_bytes(mis_fm, MISF_CIGS_DATA_TABLE_START, flash_data_ptr->bytes, PACKET_SIZE);
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
    fprintf(PC, "\t| MISF | PICLOG     | Use Counter      : 0x%08LX\r\n", piclog_data.used_counter);
    fprintf(PC, "\t| MISF | PICLOG     | Uncopyed Counter : 0x%08LX\r\n", piclog_data.uncopied_counter);
    fprintf(PC, "\t| MISF | PICLOG     | Reserve Counter1 : 0x%02X\r\n", piclog_data.reserve_counter1);
    fprintf(PC, "\t| MISF | PICLOG     | Reserve Counter2 : 0x%02X\r\n", piclog_data.reserve_counter2);
    fprintf(PC, "\t| MISF | ENVIRO     | Use Counter      : 0x%08LX\r\n", environment_data.used_counter);
    fprintf(PC, "\t| MISF | ENVIRO     | Uncopyed Counter : 0x%08LX\r\n", environment_data.uncopied_counter);
    fprintf(PC, "\t| MISF | ENVIRO     | Reserve Counter1 : 0x%02X\r\n", environment_data.reserve_counter1);
    fprintf(PC, "\t| MISF | ENVIRO     | Reserve Counter2 : 0x%02X\r\n", environment_data.reserve_counter2);
    delay_ms(10);
    fprintf(PC, "\t| MISF | IV1 HEADER | Use Counter      : 0x%08LX\r\n", iv1_header.used_counter);
    fprintf(PC, "\t| MISF | IV1 HEADER | Uncopyed Counter : 0x%08LX\r\n", iv1_header.uncopied_counter);
    fprintf(PC, "\t| MISF | IV1 HEADER | Reserve Counter1 : 0x%02X\r\n", iv1_header.reserve_counter1);
    fprintf(PC, "\t| MISF | IV1 HEADER | Reserve Counter2 : 0x%02X\r\n", iv1_header.reserve_counter2);
    fprintf(PC, "\t| MISF | IV1 DATA   | Use Counter      : 0x%08LX\r\n", iv1_data.used_counter);
    fprintf(PC, "\t| MISF | IV1 DATA   | Uncopyed Counter : 0x%08LX\r\n", iv1_data.uncopied_counter);
    fprintf(PC, "\t| MISF | IV1 DATA   | Reserve Counter1 : 0x%02X\r\n", iv1_data.reserve_counter1);
    fprintf(PC, "\t| MISF | IV1 DATA   | Reserve Counter2 : 0x%02X\r\n", iv1_data.reserve_counter2);
    delay_ms(10);
    fprintf(PC, "\t| MISF | IV2 HEADER | Use Counter      : 0x%08LX\r\n", iv2_header.used_counter);
    fprintf(PC, "\t| MISF | IV2 HEADER | Uncopyed Counter : 0x%08LX\r\n", iv2_header.uncopied_counter);
    fprintf(PC, "\t| MISF | IV2 HEADER | Reserve Counter1 : 0x%02X\r\n", iv2_header.reserve_counter1);
    fprintf(PC, "\t| MISF | IV2 HEADER | Reserve Counter2 : 0x%02X\r\n", iv2_header.reserve_counter2);
    fprintf(PC, "\t| MISF | IV2 DATA   | Use Counter      : 0x%08LX\r\n", iv2_data.used_counter);
    fprintf(PC, "\t| MISF | IV2 DATA   | Uncopyed Counter : 0x%08LX\r\n", iv2_data.uncopied_counter);
    fprintf(PC, "\t| MISF | IV2 DATA   | Reserve Counter1 : 0x%02X\r\n", iv2_data.reserve_counter1);
    fprintf(PC, "\t| MISF | IV2 DATA   | Reserve Counter2 : 0x%02X\r\n", iv2_data.reserve_counter2);
    delay_ms(10);
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
   else if (mission_id == CIGS_IV1_HEADER)
   {
      mis_struct.start_address = MISF_CIGS_IV_HEADER_START;
      mis_struct.end_address   = MISF_CIGS_IV_HEADER_END;
   }
   else if (mission_id == CIGS_IV1_DATA)
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
    else if (mission_id == CIGS_IV1_HEADER)
    {
        mis_write_struct.start_address = MISF_CIGS_IV_HEADER_START + iv1_header.used_counter - iv1_header.uncopied_counter;
        mis_write_struct.size = iv1_header.uncopied_counter;
    }
    else if (mission_id == CIGS_IV1_DATA)
    {
        mis_write_struct.start_address = MISF_CIGS_IV_DATA_START + iv1_data.used_counter - iv1_data.uncopied_counter;
        mis_write_struct.size = iv1_data.uncopied_counter;
    }
    return mis_write_struct;
}
// End of file
