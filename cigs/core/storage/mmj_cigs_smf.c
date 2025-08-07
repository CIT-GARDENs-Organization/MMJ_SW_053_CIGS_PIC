#include "mmj_cigs_smf.h"  
#include "mmj_cigs_flash.h"
#include "../../../lib/tool/smf_queue.h"         // システム設定 
#include "../../../lib/tool/calc_tools.h"        // SMF処理関数 
#include "../../../lib/tool/mmj_smf_memorymap.h" 

#define MAX_READ_SIZE 64
#define SIZE_AREA_SIZE 0x1000
#define SUBSECTOR_SIZE 0x1000 // (4096bytes)
#define CRC_RETRY_COUNT 5     // CRC検証のリトライ回数

void read_smf_header()
{
    int8 read_data[PACKET_SIZE];
    int8 retry_count;
    
    // 統合管理システムから読み込み操作をキューに追加
    FlashOperationStruct read_op;
    read_op.mission_id = 0x01;
    read_op.func_type = SMF_READ;
    read_op.src = CIGS_DATA_TABLE_START_ADDRESS;
    read_op.size = PACKET_SIZE;
    read_op.manager = get_misf_smf_manager(0x01);
    enqueue_flash_operation(&read_op);
    
    for (retry_count = 0; retry_count < CRC_RETRY_COUNT; retry_count++)
    {
        read_data_bytes(smf, CIGS_DATA_TABLE_START_ADDRESS, read_data, PACKET_SIZE);
        if (is_crc_valid(read_data, PACKET_SIZE-1))
        {
            printf("CRC verification passed on attempt %u\r\n", retry_count + 1);
            break;
        }
    }
    param.meas.used_size = lsb_array_to_int32(read_data, 0);
    param.meas.loop_counter = read_data[4];
    param.piclog.used_size = lsb_array_to_int32(read_data, 5);
    param.piclog.loop_counter = read_data[9];
}

void write_smf_header()
{
    int8 writedata[PACKET_SIZE];

    // ヘッダー情報をwritedataに設定
    int32_to_be_array(param.meas.used_size, writedata, 0);
    writedata[4] = param.meas.loop_counter;
    int32_to_be_array(param.piclog.used_size, writedata, 5);
    writedata[9] = param.piclog.loop_counter;

    writedata[63] = calc_crc8(writedata, PACKET_SIZE-1); // CRCを計算してバッファに書き込み

    // 統合管理システムから書き込み操作をキューに追加
    FlashOperationStruct write_op;
    write_op.mission_id = 0x01;
    write_op.func_type = SMF_WRITE;
    write_op.write_mode = SMF_WRITE_OVERWRITE;
    write_op.source_type = SOURCE_MISF_MANUAL;
    write_op.src = CIGS_DATA_TABLE_START_ADDRESS;
    write_op.size = PACKET_SIZE;
    write_op.manager = get_misf_smf_manager(0x01);
    enqueue_flash_operation(&write_op);

    int8 readdata[PACKET_SIZE];
    int8 retry_count;
    int1 crc_valid = 0;
    for (retry_count = 0; retry_count < CRC_RETRY_COUNT; retry_count++)
    {
        subsector_4kByte_erase(smf, CIGS_DATA_TABLE_START_ADDRESS);
        write_data_bytes(smf, CIGS_DATA_TABLE_START_ADDRESS, writedata, PACKET_SIZE); // ヘッダーを書き込み
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


void smf_write(SmfDataStruct *smf_data)
{
    fprintf(PC, "\r\n____________________\r\n");
    fprintf(PC, "___Start copy_data__\r\n");
    
    int8 buffer[PACKET_SIZE];

    MissionTypeStruct mission_type = getMissionTypeStruct(smf_data->mission_id);
    status[1] = smf_data->mission_id; // `i` is assigned mis mcu status flag. so mission_flag start `i+1`
    unsigned int32 mis_start_address = mission_type.start_address;
    unsigned int32 mis_end_address = mission_type.end_address;
    unsigned int32 write_src = smf_data->src;
    unsigned int32 write_size = smf_data->size;
    fprintf(PC, "In SMF mission data start   address: %LX\r\n", mis_start_address);
    fprintf(PC, "In SMF mission data end     address: %LX\r\n", mis_end_address);
    fprintf(PC, "In MIS FM Write source data address: %LX\r\n", write_src);
    fprintf(PC, "In MIS FM Write data size          : %lu (0x%lx)\r\n\r\n", write_size, write_size);

    if (!is_connect(mis_fm))
    {
        fprintf(PC, "Error: MIS FM is not connected\r\n");
    }    

    if (!is_connect(smf))
    {
        fprintf(PC, "Error: SMF is not connected\r\n");
    }    
    /*
    while (!is_connect(smf))
    {
        fprintf(PC, "Error: SMF is not connected\r\n");
        delay_ms(100);
    }
    */

    // read size area with CRC verification retry
    read_smf_header();

    // PartitionParamから値を取得
    int32 used_size = param.meas.used_size;
    int8 loop_count = param.meas.loop_counter;
    fprintf(PC, "Size area read\r\n");
    fprintf(PC, "used_size = %ld (src 0x%08LX)\r\n", used_size, mis_start_address);
    fprintf(PC, "loop count= %d  (src 0x%08LX)\r\n\r\n", loop_count, mis_start_address + 4);
    

    // Calculate data write address and check for wrap-around
    unsigned int32 data_region_start = mis_start_address + SUBSECTOR_SIZE;
    unsigned int32 data_region_end = mis_end_address;
    unsigned int32 data_region_size = data_region_end - data_region_start;
    unsigned int32 data_write_addr = data_region_start + used_size;


    // Delete the first part in advance in case of looping
    if ((used_size + write_size) > data_region_size)
    {
        fprintf(PC, "Wrap triggered: Resetting data_write_addr to start\r\n");
        loop_count++;
        used_size = 0;
        data_write_addr = data_region_start;
        
        // Reset counters when loop occurs
        reset_misf_counters(smf_data->mission_id);
        
        unsigned int32 erase_ptr = data_region_start;
        while (erase_ptr < data_region_end)
        {
            subsector_4kByte_erase(smf, erase_ptr);
            erase_ptr += SUBSECTOR_SIZE;     // tips: `value += 0x1000` means add up 4KB (0x1000 = 0d4096)
        }
    }


    // Erase the space you will be writing in now
    unsigned int32 erase_start = data_write_addr & ~0xFFF;       // tips: `value & ~0xFFF` means alignment 4KB.
    unsigned int32 erase_end = (data_write_addr + write_size + 0xFFF) & ~0xFFF;
    for (unsigned int32 addr = erase_start; addr < erase_end && addr < mis_end_address; addr += SUBSECTOR_SIZE)
        subsector_4kByte_erase(smf, addr);

    unsigned int32 remaining = write_size;
    unsigned int32 src_addr = write_src;
    while (remaining > 0)
    {
        unsigned int16 chunk = (remaining > MAX_READ_SIZE) ? MAX_READ_SIZE : remaining; //  = max(MAX_READ_SIZE, rest write_size)

        read_data_bytes(mis_fm, src_addr, buffer, chunk);
        write_data_bytes(smf, data_write_addr, buffer, chunk);
        for (unsigned int32 i = 0; i < chunk; i++)
        {
            fprintf(PC, "%02X ", buffer[i]);
        }
        src_addr += chunk;
        data_write_addr += chunk;
        used_size += chunk;
        remaining -= chunk;
    }
    // write size area
    write_smf_header();
    
    // Update MISF counters for transferred data
    update_misf_counters(smf_data->mission_id, write_size);
    
    fprintf(PC, "used_size = %ld\r\n", used_size);
    fprintf(PC, "loop_count = %u\r\n\r\n", loop_count);

    fprintf(PC, "\r\n___End copy_data____\r\n");
    fprintf(PC, "____________________\r\n\r\n");
}

void smf_read(SmfDataStruct *smf_data)
{
    fprintf(PC, "\r\n____________________\r\n");
    fprintf(PC, "___Start SMF Read____\r\n");

    int8 buffer[PACKET_SIZE];

    MissionTypeStruct mission_type = getMissionTypeStruct(smf_data->mission_id);
    status[1] = smf_data->mission_id; // `i` is assigned mis mcu status flag. so mission_flag start `i+1`
    unsigned int32 read_src = smf_data->src;
    unsigned int32 read_size = smf_data->size;
    fprintf(PC, "In SMF Read source data address: %LX\r\n", read_src);
    fprintf(PC, "In SMF Read data size          : %lu (0x%lx)\r\n\r\n", read_size, read_size);

    if (!is_connect(smf)){
        fprintf(PC, "Error: SMF is not connected\r\n");
    }
    fprintf(PC, "READ DATA FROM SMF...\r\n");
    for (unsigned int32 addr = read_src; addr < read_src + read_size; addr += PACKET_SIZE)
    {
        read_data_bytes(smf, addr, buffer, PACKET_SIZE);
        for (unsigned int32 i = 0; i < PACKET_SIZE; i++)
        {
            fprintf(PC, "%02X ", buffer[i]);
        }
        fprintf(PC, "\r\n");
    }
    fprintf(PC, "\r\n___End SMF Read____\r\n");
    fprintf(PC, "____________________\r\n\r\n");
}

void smf_erase(SmfDataStruct *smf_data)
{
    fprintf(PC, "\r\n____________________\r\n");
    fprintf(PC, "___Start smf_erase____\r\n");

    MissionTypeStruct mission_type = getMissionTypeStruct(smf_data->mission_id);
    status[1] = smf_data->mission_id; // `i` is assigned mis mcu status flag. so mission_flag start `i+1`
    unsigned int32 mis_start_address = mission_type.start_address;
    unsigned int32 mis_end_address = mission_type.end_address;
    unsigned int32 erase_src = smf_data->src;
    unsigned int32 erase_size = smf_data->size;
    fprintf(PC, "In SMF mission data start   address: %LX\r\n", mis_start_address);
    fprintf(PC, "In SMF mission data end     address: %LX\r\n", mis_end_address);
    fprintf(PC, "In SMF Erase source data address: %LX\r\n", erase_src);
    fprintf(PC, "In SMF Erase data size          : %lu (0x%lx)\r\n\r\n", erase_size, erase_size);

    if (!is_connect(smf)){
        fprintf(PC, "Error: SMF is not connected\r\n");
        return;
    }

    // Check if erase operation is within mission_type range
    if (erase_src < mis_start_address || erase_src >= mis_end_address) {
        fprintf(PC, "Error: Erase source address 0x%LX is outside mission range [0x%LX - 0x%LX]\r\n", 
                erase_src, mis_start_address, mis_end_address);
        return;
    }
    
    if ((erase_src + erase_size) > mis_end_address) {
        fprintf(PC, "Error: Erase operation would exceed mission end address 0x%LX\r\n", mis_end_address);
        fprintf(PC, "Limiting erase size to stay within mission bounds\r\n");
        erase_size = mis_end_address - erase_src;
    }

    fprintf(PC, "Erase operation validated within mission range\r\n");
    subsector_4kByte_erase(smf, erase_src);
    
    fprintf(PC, "\r\n___End smf_erase____\r\n");
    fprintf(PC, "____________________\r\n\r\n");
}

// mission_idから対応するSMF_PARTITIONを取得する関数
SMF_PARTITION* get_smf_partition_by_mission_id(int8 mission_id)
{
    switch(mission_id)
    {
        case 0x01:  // CIGS_MEASURE_DATA
            return &param.meas;
        case 0x02:  // CIGS_PICLOG
            return &param.piclog;
        default:
            fprintf(PC, "Error: Unknown mission_id: %02X\r\n", mission_id);
            return 0x00;  // NULL pointer
    }
}

// mission_idに対応するパーティション情報を更新する関数
void update_smf_partition_by_mission_id(int8 mission_id, int32 used_size, int32 loop_counter)
{
    SMF_PARTITION* partition = get_smf_partition_by_mission_id(mission_id);
    if (partition != 0x00)
    {
        partition->used_size = used_size;
        partition->loop_counter = loop_counter;
        fprintf(PC, "Updated partition for mission_id %02X: used_size=%ld, loop_counter=%ld\r\n", 
                mission_id, used_size, loop_counter);
    }
}

// カウンター更新関数
void update_misf_counters(int8 mission_id, int32 transfer_size)
{
    switch(mission_id)
    {
        case 0x01: // CIGS_MEASURE_DATA
            misf_meas_uncopyed_counter += transfer_size;
            fprintf(PC, "MISF Counter Update - Measurement: +%ld, Total: %lu\r\n", 
                    transfer_size, misf_meas_uncopyed_counter);
            break;
            
        case 0x02: // CIGS_PICLOG  
            misf_piclog_uncopyed_counter += transfer_size;
            fprintf(PC, "MISF Counter Update - Piclog: +%ld, Total: %lu\r\n", 
                    transfer_size, misf_piclog_uncopyed_counter);
            break;
            
        default:
            fprintf(PC, "Warning: Unknown mission_id %02X for counter update\r\n", mission_id);
            break;
    }
}

// カウンター初期化関数
void reset_misf_counters(int8 mission_id)
{
    switch(mission_id)
    {
        case 0x01: // CIGS_MEASURE_DATA
            misf_meas_uncopyed_counter = 0;
            fprintf(PC, "Reset misf_meas_uncopyed_counter\r\n");
            break;
            
        case 0x02: // CIGS_PICLOG
            misf_piclog_uncopyed_counter = 0;
            fprintf(PC, "Reset misf_piclog_uncopyed_counter\r\n");
            break;
            
        default:
            fprintf(PC, "Warning: Unknown mission_id %02X for counter reset\r\n", mission_id);
            break;
    }
}

// カウンター状態表示関数
void print_misf_counter_status(int8 mission_id)
{
    switch(mission_id)
    {
        case 0x01: // CIGS_MEASURE_DATA
            fprintf(PC, "MISF Measurement Counters:\r\n");
            fprintf(PC, "  Use Counter: %lu\r\n", misf_meas_use_counter);
            fprintf(PC, "  Uncopied Counter: %lu\r\n", misf_meas_uncopyed_counter);
            fprintf(PC, "  Loop Counter: %u\r\n", misf_meas_loop_counter);
            break;
            
        case 0x02: // CIGS_PICLOG
            fprintf(PC, "MISF Piclog Counters:\r\n");
            fprintf(PC, "  Use Counter: %lu\r\n", misf_piclog_use_counter);
            fprintf(PC, "  Uncopied Counter: %lu\r\n", misf_piclog_uncopyed_counter);
            fprintf(PC, "  Loop Counter: %u\r\n", misf_piclog_loop_counter);
            fprintf(PC, "  Write Counter: %u\r\n", misf_piclog_write_counter);
            break;
            
        default:
            fprintf(PC, "Warning: Unknown mission_id %02X for counter status\r\n", mission_id);
            break;
    }
}

