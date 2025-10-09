#include "sato_flash.h"                           
#include "../hal/sato_config.h"             
#include "../lib/device/mt25q.h"                
#include "../lib/tool/calc_tools.h"             
#include "../lib/tool/smf_queue.h"              

const ADDRESS_AREA_T MISF_ADDRESS_TABLE[FLASH_ID_COUNT] = {
    { MISF_CIGS_DATA_TABLE_START,   MISF_CIGS_DATA_TABLE_END,   MISF_CIGS_DATA_TABLE_SIZE },
    { MISF_CIGS_PICLOG_START,       MISF_CIGS_PICLOG_END,       MISF_CIGS_PICLOG_SIZE },
    { MISF_CIGS_ENVIRO_START,       MISF_CIGS_ENVIRO_END,       MISF_CIGS_ENVIRO_SIZE },
    { MISF_CIGS_IV1_HEADER_START,   MISF_CIGS_IV1_HEADER_END,   MISF_CIGS_IV1_HEADER_SIZE },
    { MISF_CIGS_IV1_DATA_START,     MISF_CIGS_IV1_DATA_END,     MISF_CIGS_IV1_DATA_SIZE },
    { MISF_CIGS_IV2_HEADER_START,   MISF_CIGS_IV2_HEADER_END,   MISF_CIGS_IV2_HEADER_SIZE },
    { MISF_CIGS_IV2_DATA_START,     MISF_CIGS_IV2_DATA_END,     MISF_CIGS_IV2_DATA_SIZE }
};

FlashCounter_t misf_counter_table[FLASH_ID_COUNT];
FlashCounter_t *misf_counter_table_ptr[FLASH_ID_COUNT] = {&misf_counter_table[0], &misf_counter_table[1], &misf_counter_table[2], &misf_counter_table[3], &misf_counter_table[4], &misf_counter_table[5], &misf_counter_table[6]};

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
    fprintf(PC, "\t[MIS FM] %08LX\r\n", MISF_ADDRESS_TABLE[FLASH_ID_DATA_TABLE].start);
    read_data_bytes(mis_fm, MISF_ADDRESS_TABLE[FLASH_ID_DATA_TABLE].start, read_header_ptr->bytes, PACKET_SIZE);

    // CRC 検証
    if (read_header_ptr->packet.crc != calc_crc8(read_header_ptr->bytes, PACKET_SIZE - 1)) {
        fprintf(PC, "\t[MIS FM] CRC error -> initialize counters\r\n");
        
    }
    misf_counter_table[FLASH_ID_PICLOG]       = read_header_ptr->packet.payload.logdata.piclog;
    misf_counter_table[FLASH_ID_ENVIRONMENT]  = read_header_ptr->packet.payload.logdata.environment;
    misf_counter_table[FLASH_ID_IV1_HEADER]   = read_header_ptr->packet.payload.logdata.iv1_header;
    misf_counter_table[FLASH_ID_IV1_DATA]     = read_header_ptr->packet.payload.logdata.iv1_data;
    misf_counter_table[FLASH_ID_IV2_HEADER]   = read_header_ptr->packet.payload.logdata.iv2_header;
    misf_counter_table[FLASH_ID_IV2_DATA]     = read_header_ptr->packet.payload.logdata.iv2_data;

    print_misf_address_table();

    fprintf(PC, "\tComplete\r\n");
}

void print_misf_address_table()
{
    fprintf(PC, "\tField\t\tCounterName\tValue\r\n");
    fprintf(PC, "\t------------\t-------------\t-------------\r\n");
    fprintf(PC, "\tPICLOG\t\tUsed\t\t0x%08LX\r\n", misf_counter_table[FLASH_ID_PICLOG].used_counter);
    fprintf(PC, "\t\t\tUncopyed\t0x%08LX\r\n", misf_counter_table[FLASH_ID_PICLOG].uncopied_counter);
    fprintf(PC, "\t\t\tReserve\t\t0x%02X/ 0x%02X\r\n", misf_counter_table[FLASH_ID_PICLOG].reserve_counter1,misf_counter_table[FLASH_ID_PICLOG].reserve_counter2);
    fprintf(PC, "\tENVIRO\t\tUsed\t\t0x%08LX\r\n", misf_counter_table[FLASH_ID_ENVIRONMENT].used_counter);
    fprintf(PC, "\t\t\tUncopyed\t0x%08LX\r\n", misf_counter_table[FLASH_ID_ENVIRONMENT].uncopied_counter);
    fprintf(PC, "\t\t\tReserve\t\t0x%02X/ 0x%02X\r\n", misf_counter_table[FLASH_ID_ENVIRONMENT].reserve_counter1,misf_counter_table[FLASH_ID_ENVIRONMENT].reserve_counter2);
    fprintf(PC, "\tIV1 HEADER\tUsed\t\t0x%08LX\r\n", misf_counter_table[FLASH_ID_IV1_HEADER].used_counter);
    fprintf(PC, "\t\t\tUncopyed\t0x%08LX\r\n", misf_counter_table[FLASH_ID_IV1_HEADER].uncopied_counter);
    fprintf(PC, "\t\t\tReserve\t\t0x%02X/ 0x%02X\r\n", misf_counter_table[FLASH_ID_IV1_HEADER].reserve_counter1,misf_counter_table[FLASH_ID_IV1_HEADER].reserve_counter2);
    fprintf(PC, "\tIV1 DATA\tUsed\t\t0x%08LX\r\n", misf_counter_table[FLASH_ID_IV1_DATA].used_counter);
    fprintf(PC, "\t\t\tUncopyed\t0x%08LX\r\n", misf_counter_table[FLASH_ID_IV1_DATA].uncopied_counter);
    fprintf(PC, "\t\t\tReserve\t\t0x%02X/ 0x%02X\r\n", misf_counter_table[FLASH_ID_IV1_DATA].reserve_counter1,misf_counter_table[FLASH_ID_IV1_DATA].reserve_counter2);
    fprintf(PC, "\tIV2 HEADER\tUsed\t\t0x%08LX\r\n", misf_counter_table[FLASH_ID_IV2_HEADER].used_counter);
    fprintf(PC, "\t\t\tUncopyed\t0x%08LX\r\n", misf_counter_table[FLASH_ID_IV2_HEADER].uncopied_counter);
    fprintf(PC, "\t\t\tReserve\t\t0x%02X/ 0x%02X\r\n", misf_counter_table[FLASH_ID_IV2_HEADER].reserve_counter1,misf_counter_table[FLASH_ID_IV2_HEADER].reserve_counter2);
    fprintf(PC, "\tIV2 DATA\tUsed\t\t0x%08LX\r\n", misf_counter_table[FLASH_ID_IV2_DATA].used_counter);
    fprintf(PC, "\t\t\tUncopyed\t0x%08LX\r\n", misf_counter_table[FLASH_ID_IV2_DATA].uncopied_counter);
    fprintf(PC, "\t\t\tReserve\t\t0x%02X/ 0x%02X\r\n", misf_counter_table[FLASH_ID_IV2_DATA].reserve_counter1,misf_counter_table[FLASH_ID_IV2_DATA].reserve_counter2);
    fprintf(PC, "\t==================================================\r\n");
}

void misf_write_data(FlashDataId_t id, unsigned int8 *data_ptr, unsigned int16 size)
{
    /*
    fprintf(PC, "[MIS FM] Write Data Size:%lu\r\n", size);
    fprintf(PC, "%08LX\r\n", MISF_ADDRESS_TABLE[id].start);
    fprintf(PC, "%08LX\r\n", misf_counter_table[id].used_counter);
    fprintf(PC, "%08LX\r\n", misf_counter_table[id].reserve_counter1);
    for (unsigned int32 j = 0; j < size; j++) {
        fprintf(PC, "%02X ", data_ptr[j]);
    }
    fprintf(PC, "\r\n");
    */
    // パケット境界チェック
    if (size == PACKET_SIZE){
        // fprintf(PC, "PACKET SIZE 64\r\n");
        unsigned int32 write_address = MISF_ADDRESS_TABLE[id].start + misf_counter_table[id].used_counter;
        write_data_bytes(mis_fm, write_address, data_ptr, size);
        // for (unsigned int32 j = 0; j < size; j++) {
            // fprintf(PC, "%02X ", data_ptr[j]);
        // }
        misf_counter_table[id].used_counter     += size;
        misf_counter_table[id].uncopied_counter += size;

        return;
    }
    
    else if (misf_counter_table[id].reserve_counter1 + size == PACKET_SIZE-1) {
        unsigned int8 blank_size = PACKET_SIZE - 1 - misf_counter_table[id].reserve_counter1;
        // fprintf(PC, "0x%02X", blank_size);
        unsigned int8 blank_data[PACKET_SIZE];
        memset(blank_data, 0, blank_size);

        // アドレス再計算
        unsigned int32 write_address = MISF_ADDRESS_TABLE[id].start + misf_counter_table[id].used_counter;

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
        misf_counter_table[id].used_counter     += blank_size + 1;
        misf_counter_table[id].uncopied_counter += blank_size + 1;
        misf_counter_table[id].reserve_counter1  = 0;
        return;

    }else  if(misf_counter_table[id].reserve_counter1 + size == PACKET_SIZE-1){
        // データ書き込み
        unsigned int32 write_address = MISF_ADDRESS_TABLE[id].start + misf_counter_table[id].used_counter;
        write_data_bytes(mis_fm, write_address, data_ptr, size);
        // fprintf(PC, "ADDRESS 0x%08LX DATA ", write_address);
        // for (unsigned int32 j = 0; j < size; j++) {
            // fprintf(PC, "%02X ", data_ptr[j]);
        // }
        // fprintf(PC, "\Sr\n");

        misf_counter_table[id].used_counter     += size;
        misf_counter_table[id].uncopied_counter += size;
        unsigned int8 crc = calc_crc8(data_ptr, size);

        // CRC 書き込み
        write_address = MISF_ADDRESS_TABLE[id].start + misf_counter_table[id].used_counter;
        write_data_bytes(mis_fm, write_address, &crc, 1);

        // カウンタ更新
        misf_counter_table[id].used_counter     += 1;
        misf_counter_table[id].uncopied_counter += 1;
        misf_counter_table[id].reserve_counter1  = 0;

        // アドレステーブル更新
        // misf_update_address_area();
        return;
    }

    // データ書き込み
    unsigned int32 write_address = MISF_ADDRESS_TABLE[id].start + misf_counter_table[id].used_counter;
    // fprintf(PC, "ADDRESS 0x%08LX DATA ", write_address);
    write_data_bytes(mis_fm, write_address, data_ptr, size);

    // カウンタ更新
    misf_counter_table[id].used_counter     += size;
    misf_counter_table[id].uncopied_counter += size;
    misf_counter_table[id].reserve_counter1 += size;

    // アドレステーブル更新
    // misf_update_address_area();
}

FlashData_t make_flash_data_table(void)
{
    FlashData_t flash_data;
    FlashData_t *flash_data_ptr = &flash_data;

    // 初期化
    memset(flash_data_ptr->bytes, 0, sizeof(flash_data_ptr->bytes));

    flash_data_ptr->packet.payload.logdata.piclog = misf_counter_table[FLASH_ID_PICLOG];
    flash_data_ptr->packet.payload.logdata.environment = misf_counter_table[FLASH_ID_ENVIRONMENT];
    flash_data_ptr->packet.payload.logdata.iv1_header = misf_counter_table[FLASH_ID_IV1_HEADER];
    flash_data_ptr->packet.payload.logdata.iv1_data = misf_counter_table[FLASH_ID_IV1_DATA];
    flash_data_ptr->packet.payload.logdata.iv2_header = misf_counter_table[FLASH_ID_IV2_HEADER];
    flash_data_ptr->packet.payload.logdata.iv2_data = misf_counter_table[FLASH_ID_IV2_DATA];

    // CRC 計算
    flash_data_ptr->packet.crc = calc_crc8(flash_data_ptr->bytes, PACKET_SIZE - 1);

    return flash_data;
}

void misf_update_address_area(void)
{
    FlashData_t flash_data = make_flash_data_table();
    FlashData_t *flash_data_ptr = &flash_data;

    subsector_4kByte_erase(mis_fm, MISF_ADDRESS_TABLE[FLASH_ID_DATA_TABLE].start);
    write_data_bytes(mis_fm, MISF_ADDRESS_TABLE[FLASH_ID_DATA_TABLE].start, flash_data_ptr->bytes, PACKET_SIZE);
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



// End of file