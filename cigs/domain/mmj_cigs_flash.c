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

const ADDRESS_AREA_T MISF_ADDRESS_TABLE[FLASH_ID_COUNT] = {
    { MISF_CIGS_DATA_TABLE_START,   MISF_CIGS_DATA_TABLE_END,   MISF_CIGS_DATA_TABLE_SIZE },
    { MISF_CIGS_PICLOG_START,       MISF_CIGS_PICLOG_END,       MISF_CIGS_PICLOG_SIZE },
    { MISF_CIGS_ENVIRO_START,       MISF_CIGS_ENVIRO_END,       MISF_CIGS_ENVIRO_SIZE },
    { MISF_CIGS_IV1_HEADER_START,   MISF_CIGS_IV1_HEADER_END,   MISF_CIGS_IV1_HEADER_SIZE },
    { MISF_CIGS_IV1_DATA_START,     MISF_CIGS_IV1_DATA_END,     MISF_CIGS_IV1_DATA_SIZE },
    { MISF_CIGS_IV2_HEADER_START,   MISF_CIGS_IV2_HEADER_END,   MISF_CIGS_IV2_HEADER_SIZE },
    { MISF_CIGS_IV2_DATA_START,     MISF_CIGS_IV2_DATA_END,     MISF_CIGS_IV2_DATA_SIZE }
};

FlashCounter_t flash_counter_table[FLASH_ID_COUNT];
FlashCounter_t *flash_counter_table_ptr[FLASH_ID_COUNT] = {&flash_counter_table[0], &flash_counter_table[1], &flash_counter_table[2], &flash_counter_table[3], &flash_counter_table[4], &flash_counter_table[5], &flash_counter_table[6]};



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
        misf_update_address_area();   // 初期テーブル書込み
        print_flash_status();
        return;
        */
    }
    flash_counter_table[FLASH_ID_PICLOG]       = read_header_ptr->packet.payload.logdata.piclog;
    flash_counter_table[FLASH_ID_ENVIRONMENT]  = read_header_ptr->packet.payload.logdata.environment;
    flash_counter_table[FLASH_ID_IV1_HEADER]   = read_header_ptr->packet.payload.logdata.iv1_header;
    flash_counter_table[FLASH_ID_IV1_DATA]     = read_header_ptr->packet.payload.logdata.iv1_data;
    flash_counter_table[FLASH_ID_IV2_HEADER]   = read_header_ptr->packet.payload.logdata.iv2_header;
    flash_counter_table[FLASH_ID_IV2_DATA]     = read_header_ptr->packet.payload.logdata.iv2_data;

    print_misf_address_table();

    fprintf(PC, "\tComplete\r\n");
}

void print_misf_address_table()
{
    fprintf(PC, "\tField\t\tCounterName\tValue\r\n");
    fprintf(PC, "\t------------\t-------------\t-------------\r\n");
    fprintf(PC, "\tPICLOG\t\tUsed\t\t0x%08LX\r\n", flash_counter_table[FLASH_ID_PICLOG].used_counter);
    fprintf(PC, "\t\t\tUncopyed\t0x%08LX\r\n", flash_counter_table[FLASH_ID_PICLOG].uncopied_counter);
    fprintf(PC, "\t\t\tReserve\t\t0x%02X/ 0x%02X\r\n", flash_counter_table[FLASH_ID_PICLOG].reserve_counter1,flash_counter_table[FLASH_ID_PICLOG].reserve_counter2);
    fprintf(PC, "\tENVIRO\t\tUsed\t\t0x%08LX\r\n", flash_counter_table[FLASH_ID_ENVIRONMENT].used_counter);
    fprintf(PC, "\t\t\tUncopyed\t0x%08LX\r\n", flash_counter_table[FLASH_ID_ENVIRONMENT].uncopied_counter);
    fprintf(PC, "\t\t\tReserve\t\t0x%02X/ 0x%02X\r\n", flash_counter_table[FLASH_ID_ENVIRONMENT].reserve_counter1,flash_counter_table[FLASH_ID_ENVIRONMENT].reserve_counter2);
    fprintf(PC, "\tIV1 HEADER\tUsed\t\t0x%08LX\r\n", flash_counter_table[FLASH_ID_IV1_HEADER].used_counter);
    fprintf(PC, "\t\t\tUncopyed\t0x%08LX\r\n", flash_counter_table[FLASH_ID_IV1_HEADER].uncopied_counter);
    fprintf(PC, "\t\t\tReserve\t\t0x%02X/ 0x%02X\r\n", flash_counter_table[FLASH_ID_IV1_HEADER].reserve_counter1,flash_counter_table[FLASH_ID_IV1_HEADER].reserve_counter2);
    fprintf(PC, "\tIV1 DATA\tUsed\t\t0x%08LX\r\n", flash_counter_table[FLASH_ID_IV1_DATA].used_counter);
    fprintf(PC, "\t\t\tUncopyed\t0x%08LX\r\n", flash_counter_table[FLASH_ID_IV1_DATA].uncopied_counter);
    fprintf(PC, "\t\t\tReserve\t\t0x%02X/ 0x%02X\r\n", flash_counter_table[FLASH_ID_IV1_DATA].reserve_counter1,flash_counter_table[FLASH_ID_IV1_DATA].reserve_counter2);
    fprintf(PC, "\tIV2 HEADER\tUsed\t\t0x%08LX\r\n", flash_counter_table[FLASH_ID_IV2_HEADER].used_counter);
    fprintf(PC, "\t\t\tUncopyed\t0x%08LX\r\n", flash_counter_table[FLASH_ID_IV2_HEADER].uncopied_counter);
    fprintf(PC, "\t\t\tReserve\t\t0x%02X/ 0x%02X\r\n", flash_counter_table[FLASH_ID_IV2_HEADER].reserve_counter1,flash_counter_table[FLASH_ID_IV2_HEADER].reserve_counter2);
    fprintf(PC, "\tIV2 DATA\tUsed\t\t0x%08LX\r\n", flash_counter_table[FLASH_ID_IV2_DATA].used_counter);
    fprintf(PC, "\t\t\tUncopyed\t0x%08LX\r\n", flash_counter_table[FLASH_ID_IV2_DATA].uncopied_counter);
    fprintf(PC, "\t\t\tReserve\t\t0x%02X/ 0x%02X\r\n", flash_counter_table[FLASH_ID_IV2_DATA].reserve_counter1,flash_counter_table[FLASH_ID_IV2_DATA].reserve_counter2);
    fprintf(PC, "\t------------\t-------------\t-------------\r\n");
}

void misf_write_data(FlashDataId_t id, int8 *data_ptr, unsigned int16 size)
{
    // パケット境界チェック
    if (flash_counter_table[id].reserve_counter1 + size > PACKET_SIZE-1) {
        unsigned int8 blank_size = PACKET_SIZE - 1 - flash_counter_table[id].reserve_counter1;
        // fprintf(PC, "0x%02X", blank_size);
        unsigned int8 blank_data[PACKET_SIZE];
        memset(blank_data, 0, blank_size);

        // アドレス再計算
        unsigned int32 write_address = MISF_ADDRESS_TABLE[id].start + flash_counter_table[id].used_counter;

        // 未使用部分を 0 埋め
        write_data_bytes(mis_fm, write_address, blank_data, blank_size);

        // CRC 計算（本体部分だけ読む）
        unsigned int8 packet_data[PACKET_SIZE - 1];
        unsigned int32 read_address = write_address + blank_size - PACKET_SIZE - 1;
        read_data_bytes(mis_fm, read_address, packet_data, PACKET_SIZE - 1);
        unsigned int8 crc = calc_crc8(packet_data, PACKET_SIZE - 1);
        // fprintf(PC, "0x%02X", crc);
        // CRC 書き込み
        write_address += blank_size;
        write_data_bytes(mis_fm, write_address, &crc, 1);

        // カウンタ更新
        flash_counter_table[id].used_counter     += blank_size + 1;
        flash_counter_table[id].uncopied_counter += blank_size + 1;
        flash_counter_table[id].reserve_counter1  = 0;
    }
    if (flash_counter_table[id].reserve_counter1 + size == PACKET_SIZE-1){
        // データ書き込み
        unsigned int32 write_address = MISF_ADDRESS_TABLE[id].start + flash_counter_table[id].used_counter;
        write_data_bytes(mis_fm, write_address, data_ptr, size);
        fprintf(PC, "ADDRESS 0x%08LX DATA ", write_address);
        for (unsigned int32 j = 0; j < size; j++) {
            fprintf(PC, "%02X ", data_ptr[j]);
        }
        fprintf(PC, "\r\n");

        flash_counter_table[id].used_counter     += size;
        flash_counter_table[id].uncopied_counter += size;
        unsigned int8 crc = calc_crc8(data_ptr, size);

        // CRC 書き込み
        write_address = MISF_ADDRESS_TABLE[id].start + flash_counter_table[id].used_counter;
        write_data_bytes(mis_fm, write_address, &crc, 1);

        // カウンタ更新
        flash_counter_table[id].used_counter     += 1;
        flash_counter_table[id].uncopied_counter += 1;
        flash_counter_table[id].reserve_counter1  = 0;

        // アドレステーブル更新
        misf_update_address_area();
        return;
    }

    // データ書き込み
    unsigned int32 write_address = MISF_ADDRESS_TABLE[id].start + flash_counter_table[id].used_counter;
    write_data_bytes(mis_fm, write_address, data_ptr, size);

    // カウンタ更新
    flash_counter_table[id].used_counter     += size;
    flash_counter_table[id].uncopied_counter += size;
    flash_counter_table[id].reserve_counter1 += size;

    // アドレステーブル更新
    misf_update_address_area();
}

FlashData_t make_flash_data_table(void)
{
    FlashData_t flash_data;
    FlashData_t *flash_data_ptr = &flash_data;

    // 初期化
    memset(flash_data_ptr->bytes, 0, sizeof(flash_data_ptr->bytes));

    flash_data_ptr->packet.payload.logdata.piclog = flash_counter_table[FLASH_ID_PICLOG];
    flash_data_ptr->packet.payload.logdata.environment = flash_counter_table[FLASH_ID_ENVIRONMENT];
    flash_data_ptr->packet.payload.logdata.iv1_header = flash_counter_table[FLASH_ID_IV1_HEADER];
    flash_data_ptr->packet.payload.logdata.iv1_data = flash_counter_table[FLASH_ID_IV1_DATA];
    flash_data_ptr->packet.payload.logdata.iv2_header = flash_counter_table[FLASH_ID_IV2_HEADER];
    flash_data_ptr->packet.payload.logdata.iv2_data = flash_counter_table[FLASH_ID_IV2_DATA];

    // CRC 計算
    flash_data_ptr->packet.crc = calc_crc8(flash_data_ptr->bytes, PACKET_SIZE - 1);

    return flash_data;
}

void misf_update_address_area(void)
{
    FlashData_t flash_data = make_flash_data_table();
    FlashData_t *flash_data_ptr = &flash_data;

    sector_erase(mis_fm, MISF_CIGS_DATA_TABLE_START);
    write_data_bytes(mis_fm, MISF_CIGS_DATA_TABLE_START, flash_data_ptr->bytes, PACKET_SIZE);
}


// PICLOG 更新後に呼ばれる関数例 (piclog_write 内で既に misf_update_address_area 呼んでいる場合は不要)
static void update_piclog_counters_after_write(unsigned int16 wrote_size)
{
    piclog_data.used_counter     += wrote_size;
    piclog_data.uncopied_counter += wrote_size;
    // 必要なら wrap 処理追加
    misf_update_address_area();
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

void write_smf_header(smf_data_table_t *smf_data_table)
{

    int8 readdata[PACKET_SIZE];
    int8 retry_count;
    int1 crc_valid = 0;
    for (retry_count = 0; retry_count < CRC_RETRY_COUNT; retry_count++)
    {
        subsector_4kByte_erase(smf, CIGS_DATA_TABLE_START_ADDRESS);
        write_data_bytes(smf, CIGS_DATA_TABLE_START_ADDRESS, smf_data_table->bytes, PACKET_SIZE);
        read_data_bytes(smf, CIGS_DATA_TABLE_START_ADDRESS, readdata, PACKET_SIZE);
        if (is_crc_valid(readdata, PACKET_SIZE-1))
        {
            crc_valid = 1; // FIX: フラグ更新
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
      mis_struct.start_address = MISF_CIGS_IV1_HEADER_START;
      mis_struct.end_address   = MISF_CIGS_IV1_HEADER_END;
   }
   else if (mission_id == CIGS_IV1_DATA)
   {
      mis_struct.start_address = MISF_CIGS_IV1_DATA_START;
      mis_struct.end_address   = MISF_CIGS_IV1_DATA_END;
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
        mis_write_struct.start_address = MISF_CIGS_IV1_HEADER_START + iv1_header.used_counter - iv1_header.uncopied_counter;
        mis_write_struct.size = iv1_header.uncopied_counter;
    }
    else if (mission_id == CIGS_IV1_DATA)
    {
        mis_write_struct.start_address = MISF_CIGS_IV1_DATA_START + iv1_data.used_counter - iv1_data.uncopied_counter;
        mis_write_struct.size = iv1_data.uncopied_counter;
    }
    return mis_write_struct;
}
// End of file
