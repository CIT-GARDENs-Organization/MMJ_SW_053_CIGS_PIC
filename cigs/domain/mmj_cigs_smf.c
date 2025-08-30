#include "mmj_cigs_smf.h"  
#include "mmj_cigs_flash.h"
#include "../lib/device/mt25q.h"              // Flash device definitions
#include "../lib/tool/smf_queue.h"            // システム設定 
#include "../lib/tool/calc_tools.h"           // SMF処理関数 
#include "../lib/tool/mmj_smf_memorymap.h" 

#define MAX_READ_SIZE 64
#define SIZE_AREA_SIZE 0x1000
#define SUBSECTOR_SIZE 0x1000 // (4096bytes)
#define CRC_RETRY_COUNT 5     // CRC検証のリトライ回数

// PartitionParam グローバル実体
// ヘッダ(mmj_cigs_smf.h)で extern 宣言されている param をここで定義する。
// 初期状態は 0 クリア。
PartitionParam param = {0};

void smf_data_table_init()
{
    FlashData_t smf_data_table = {0};
    calc_crc8(smf_data_table.bytes, PACKET_SIZE - 1); // CRCを計算して初期化

    write_data_bytes(smf, CIGS_DATA_TABLE_START_ADDRESS, smf_data_table.bytes, PACKET_SIZE);

}

void read_smf_header(smf_data_table_t *smf_data_table)
{
    int8 retry_count;

    read_data_bytes(smf, CIGS_DATA_TABLE_START_ADDRESS, smf_data_table->bytes, PACKET_SIZE);

    for (retry_count = 0; retry_count < CRC_RETRY_COUNT; retry_count++)
    {
        read_data_bytes(smf, CIGS_DATA_TABLE_START_ADDRESS, smf_data_table->bytes, PACKET_SIZE);
        if (is_crc_valid(smf_data_table->bytes, PACKET_SIZE-1))
        {
            printf("CRC verification passed on attempt %u\r\n", retry_count + 1);
            break;
        }
    }
}

void write_smf_header()
{

    FlashData_t flash_data = make_flash_data_table();
    unsigned int8 readdata[PACKET_SIZE];
    // int8 retry_count;
    int1 crc_valid = 0;
    for (int8 retry_count = 0; retry_count < CRC_RETRY_COUNT; retry_count++)
    {
        subsector_4kByte_erase(smf, CIGS_DATA_TABLE_START_ADDRESS);
        write_data_bytes(smf, CIGS_DATA_TABLE_START_ADDRESS, flash_data.bytes, PACKET_SIZE); // ヘッダーを書き込み
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


void smf_write(FlashOperationStruct *smf_data)
{
    fprintf(PC, "\r\n____________________\r\n");
    fprintf(PC, "_____Start copy_data____\r\n");
    
    smf_data_table_t smf_data_table;
    SmfAddressStruct smf_address = get_smf_address_struct(smf_data->mission_id);

    unsigned int32 mis_start_address = smf_address.start_address;
    unsigned int32 mis_end_address = smf_address.end_address;
    unsigned int32 write_src;
    unsigned int32 write_size;

    //アドレスと自動更新
    if (smf_data->source_type == SOURCE_MISF_UNCOPIED )
    {
        MisfAddressStruct misf_address = get_misf_address_struct(smf_data->mission_id);
        write_src = misf_address.start_address;
        write_size = iv1_data_ptr->uncopied_counter;
    }else if(smf_data->source_type == SOURCE_MISF_MANUAL)
    {
        // 手動指定データを転送
        write_src = smf_data->misf_start_addr;
        write_size = smf_data->misf_size;
    }

    // 接続確認
    if (!is_connect(mis_fm))
    {
        fprintf(PC, "Error: MIS FM is not connected\r\n");
    }    
    if (!is_connect(smf))
    {
        fprintf(PC, "Error: SMF is not connected\r\n");
    }    

    // read size area with CRC verification retry
    read_smf_header(&smf_data_table);
    int32 used_size = param.meas.used_size;
    int8 loop_count = param.meas.loop_counter;
    fprintf(PC, "Size area read\r\n");
    fprintf(PC, "smf_used_size = %ld (src 0x%08LX)\r\n", used_size, mis_start_address);
    fprintf(PC, "smf_loop count= %d  (src 0x%08LX)\r\n", loop_count, mis_start_address + 4);
    fprintf(PC, "misf_write_source = 0x%08LX\r\n", write_src);
    fprintf(PC, "misf_write_size = 0x%08LX\r\n", write_size);

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
    int8 buffer[PACKET_SIZE];
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

void smf_read(FlashOperationStruct *smf_data)
{
    fprintf(PC, "\r\n____________________\r\n");
    fprintf(PC, "___Start SMF Read____\r\n");

    int8 buffer[PACKET_SIZE];

    SmfAddressStruct smf_address = get_smf_address_struct(smf_data->mission_id);
    unsigned int32 read_address = smf_data->misf_start_addr;
    unsigned int32 read_size = smf_data->misf_size;

    fprintf(PC, "In SMF Read source data address: %LX\r\n", read_address);
    fprintf(PC, "In SMF Read data size          : %lu (0x%lx)\r\n\r\n", read_size, read_size);

    if (!is_connect(smf)){
        fprintf(PC, "Error: SMF is not connected\r\n");
    }
    fprintf(PC, "READ DATA FROM SMF...\r\n");
    for (unsigned int32 addr = read_address; addr < read_address + read_size; addr += PACKET_SIZE)
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

void smf_erase(FlashOperationStruct *smf_data)
{
    fprintf(PC, "\r\n____________________\r\n");
    fprintf(PC, "___Start smf_erase____\r\n");

    SmfAddressStruct mission_type = get_smf_address_struct(smf_data->mission_id);
    unsigned int32 erase_address = smf_data->misf_start_addr;
    unsigned int32 erase_size = smf_data->misf_size;
    fprintf(PC, "In SMF Erase source data address: %LX\r\n", erase_address);
    fprintf(PC, "In SMF Erase data size          : %lu (0x%lx)\r\n\r\n", erase_size, erase_size);

    if (!is_connect(smf)){
        fprintf(PC, "Error: SMF is not connected\r\n");
        return;
    }

    // Check if erase operation is within mission_type range
    if (erase_address < mission_type.start_address || erase_address >= mission_type.end_address) {
        fprintf(PC, "Error: Erase source address 0x%LX is outside mission range [0x%LX - 0x%LX]\r\n", 
                erase_address, mission_type.start_address, mission_type.end_address);
        return;
    }

    if ((erase_address + erase_size) > mission_type.end_address) {
        fprintf(PC, "Error: Erase operation would exceed mission end address 0x%LX\r\n", mission_type.end_address);
        fprintf(PC, "Limiting erase size to stay within mission bounds\r\n");
        erase_size = mission_type.end_address - erase_address;
    }

    fprintf(PC, "Erase operation validated within mission range\r\n");
    subsector_4kByte_erase(smf, erase_address);

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
    /*
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
    */
}

// カウンター初期化関数
void reset_misf_counters(int8 mission_id)
{
    /*
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
    */
}

// カウンター状態表示関数
void print_misf_counter_status(int8 mission_id)
{
    /*
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
    */
}
// End of file
