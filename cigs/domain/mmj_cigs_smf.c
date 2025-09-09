#include "mmj_cigs_smf.h"  
#include "mmj_cigs_flash.h"
#include "../lib/device/mt25q.h"              // Flash device definitions
#include "../lib/tool/smf_queue.h"            // システム設定 
#include "../lib/tool/calc_tools.h"           // SMF処理関数 
#include "../lib/tool/mmj_smf_memorymap.h" 

#define MAX_READ_SIZE 64
#define SIZE_AREA_SIZE 0x1000
#define SUBSECTOR_SIZE 0x1000 // (4096bytes)
#define CRC_RETRY_COUNT 5     // CRC検証のリトライ回数 (smf_queue.hの値と整合するなら一元化検討)


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
            crc_valid = 1; // FIX
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


void smf_write(FlashOperationStruct *smf_data_ptr)
{
    fprintf(PC, "\r\n_________________________\r\n");
    fprintf(PC, "_____Start copy_data____\r\n");
    
    smf_data_table_t smf_data_table;
    SmfAddressStruct smf_address = get_smf_address_struct(smf_data_ptr->mission_id);

    unsigned int32 mis_start_address = smf_address.start_address;
    unsigned int32 mis_end_address = smf_address.end_address;
    unsigned int32 write_src;
    unsigned int32 write_size;

    //アドレスと自動更新
    if (smf_data_ptr->source_type == SOURCE_MISF_UNCOPIED )
    {
        MisfAddressStruct misf_address = get_misf_address_struct(smf_data_ptr->mission_id);
        write_src = misf_address.start_address;
        switch(smf_data_ptr->mission_id){
            case CIGS_PICLOG_DATA: write_size = piclog_data_ptr->uncopied_counter; break;
            case CIGS_ENVIRO_DATA: write_size = environment_data_ptr->uncopied_counter; break;
            case CIGS_IV1_HEADER:  write_size = iv1_header_ptr->uncopied_counter; break;
            case CIGS_IV1_DATA:    write_size = iv1_data_ptr->uncopied_counter; break;
            case CIGS_IV2_HEADER:  write_size = iv2_header_ptr->uncopied_counter; break;
            case CIGS_IV2_DATA:    write_size = iv2_data_ptr->uncopied_counter; break;
            default: write_size = 0; break;
        }
    }else if(smf_data_ptr->source_type == SOURCE_MISF_MANUAL)
    {
        write_src = smf_data_ptr->misf_start_addr;
        write_size = smf_data_ptr->misf_size;
    }

    // 接続確認
    if (!is_connect(mis_fm))
    {
        fprintf(PC, "Error: MIS FM is not connected\r\n");
        return;
    }    
    if (!is_connect(smf))
    {
        fprintf(PC, "Error: SMF is not connected\r\n");
        return;
    }    

    // read size area with CRC verification retry
    read_smf_header(&smf_data_table);
    // data_table パーティションは削除。必要なら別カウンタ運用へ拡張。
    int32 used_size = 0;
    int8 loop_count = 0;
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
        
    // Reset counters when loop occurs (typo fix smf_data_ptr)
    reset_misf_counters(smf_data_ptr->mission_id);
        
        unsigned int32 erase_ptr = data_region_start;
        while (erase_ptr < data_region_end)
        {
            subsector_4kByte_erase(smf, erase_ptr);
            erase_ptr += SUBSECTOR_SIZE;     // tips: `value += 0x1000` means add up 4KB (0x1000 = 0d4096)
        }
    }


    // Erase the space you will be writing in now
    unsigned int32 erase_start = data_write_addr & ~0xFFF;       // 4KB境界へ切下げ
    unsigned int32 erase_end = (data_write_addr + write_size + 0xFFF) & ~0xFFF; // 書込末尾を4KB境界へ丸め上げ
    for (unsigned int32 addr = erase_start; addr < erase_end && addr < mis_end_address; addr += SUBSECTOR_SIZE)
        subsector_4kByte_erase(smf, addr);

    unsigned int32 remaining = write_size;
    unsigned int32 src_addr = write_src;
    int8 buffer[PACKET_SIZE];
    while (remaining > 0)
    {
        unsigned int16 chunk = (remaining > MAX_READ_SIZE) ? MAX_READ_SIZE : (unsigned int16)remaining;
        if(data_write_addr + chunk > mis_end_address){
            unsigned int32 fit = mis_end_address - data_write_addr;
            if(fit == 0){
                fprintf(PC, "[SMF] Reached end address 0x%08LX, stop writing\r\n", mis_end_address);
                break;
            }
            chunk = (unsigned int16)fit;
            fprintf(PC, "[SMF] Truncate last chunk to %u bytes at end\r\n", chunk);
        }

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
    update_misf_counters(smf_data_ptr->mission_id, write_size);
    
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
    unsigned int32 read_address = smf_data->misf_start_addr; // 呼び出し側でSMF内アドレスを設定する前提
    unsigned int32 read_size = smf_data->misf_size;

    // 境界チェック (start/end は [start_address, end_address) の半開区間想定)
    if (read_address < (unsigned int32)smf_address.start_address || read_address >= (unsigned int32)smf_address.end_address) {
        fprintf(PC, "Error: Read address 0x%08LX out of SMF mission range [0x%08LX - 0x%08LX)\r\n",
                read_address, (unsigned int32)smf_address.start_address, (unsigned int32)smf_address.end_address);
        return;
    }
    unsigned int32 mission_remaining = (unsigned int32)smf_address.end_address - read_address; // 読める残り
    if (read_size > mission_remaining) {
        fprintf(PC, "Warn: Adjust read size from %lu to %lu (mission end)\r\n", read_size, mission_remaining);
        read_size = mission_remaining;
    }
    if (read_size == 0) {
        fprintf(PC, "Info: Nothing to read (size=0)\r\n");
        return;
    }

    fprintf(PC, "In SMF Read source data address: %LX\r\n", read_address);
    fprintf(PC, "In SMF Read data size          : %lu (0x%lx)\r\n\r\n", read_size, read_size);

    if (!is_connect(smf)){
        fprintf(PC, "Error: SMF is not connected\r\n");
    }
    fprintf(PC, "READ DATA FROM SMF...\r\n");
    unsigned int32 end_address = read_address + read_size;
    for (unsigned int32 addr = read_address; addr < end_address; addr += PACKET_SIZE) {
        unsigned int16 chunk = (end_address - addr) >= PACKET_SIZE ? PACKET_SIZE : (unsigned int16)(end_address - addr);
        read_data_bytes(smf, addr, buffer, chunk);
        for (unsigned int16 i = 0; i < chunk; i++) fprintf(PC, "%02X ", buffer[i]);
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
    fprintf(PC, "[SMF] Erase address   : %LX\r\n", erase_address);
    fprintf(PC, "[SMF] Erase data size : %lu (0x%lx)\r\n\r\n", erase_size, erase_size);

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
    unsigned int32 erase_end = erase_address + erase_size;
    // 4KB境界開始へアライン
    unsigned int32 current = erase_address & ~0xFFF;
    while (current < erase_end && current < (unsigned int32)mission_type.end_address) {
        subsector_4kByte_erase(smf, current);
        current += SUBSECTOR_SIZE;
    }
    fprintf(PC, "Erased %lu bytes (aligned)\r\n", (current - (erase_address & ~0xFFF)));

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
    // MISF 側 uncopied_counter を転送サイズ分減少 (下限0)
    Flash_t *target = 0x00;
    switch(mission_id){
        case CIGS_PICLOG_DATA: target = piclog_data_ptr; break;
        case CIGS_ENVIRO_DATA: target = environment_data_ptr; break;
        case CIGS_IV1_HEADER:  target = iv1_header_ptr; break;
        case CIGS_IV1_DATA:    target = iv1_data_ptr; break;
        case CIGS_IV2_HEADER:  target = iv2_header_ptr; break;
        case CIGS_IV2_DATA:    target = iv2_data_ptr; break;
        default: break; // DATA_TABLE などは対象外
    }
    if(target){
        unsigned int32 before = target->uncopied_counter;
        if(transfer_size >= before) target->uncopied_counter = 0; else target->uncopied_counter -= transfer_size;
        fprintf(PC, "[MISF] mission %u uncopied_counter %08LX -> %08LX (-%lu)\r\n", mission_id, before, target->uncopied_counter, transfer_size);
        write_misf_address_area();
    } else {
        fprintf(PC, "[MISF] update_misf_counters: mission %u not handled\r\n", mission_id);
    }
}

// カウンター初期化関数
void reset_misf_counters(int8 mission_id)
{
    Flash_t *target = 0x00;
    switch(mission_id){
        case CIGS_PICLOG_DATA: target = piclog_data_ptr; break;
        case CIGS_ENVIRO_DATA: target = environment_data_ptr; break;
        case CIGS_IV1_HEADER:  target = iv1_header_ptr; break;
        case CIGS_IV1_DATA:    target = iv1_data_ptr; break;
        case CIGS_IV2_HEADER:  target = iv2_header_ptr; break;
        case CIGS_IV2_DATA:    target = iv2_data_ptr; break;
        default: break;
    }
    if(target){
        target->uncopied_counter = 0;
        write_misf_address_area();
        fprintf(PC, "[MISF] mission %u uncopied_counter reset\r\n", mission_id);
    }
}

// カウンター状態表示関数
void print_misf_counter_status(int8 mission_id)
{
    Flash_t *target = 0x00;
    switch(mission_id){
        case CIGS_PICLOG_DATA: target = piclog_data_ptr; break;
        case CIGS_ENVIRO_DATA: target = environment_data_ptr; break;
        case CIGS_IV1_HEADER:  target = iv1_header_ptr; break;
        case CIGS_IV1_DATA:    target = iv1_data_ptr; break;
        case CIGS_IV2_HEADER:  target = iv2_header_ptr; break;
        case CIGS_IV2_DATA:    target = iv2_data_ptr; break;
        default: break;
    }
    if(target){
        fprintf(PC, "[MISF] mission %u status: used=0x%08LX uncopied=0x%08LX r1=0x%02X r2=0x%02X\r\n",
                mission_id, target->used_counter, target->uncopied_counter,
                target->reserve_counter1, target->reserve_counter2);
    } else {
        fprintf(PC, "[MISF] print status: mission %u not handled\r\n", mission_id);
    }
}
// End of file
