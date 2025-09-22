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


const ADDRESS_AREA_T SMF_ADDRESS_TABLE[FLASH_ID_COUNT] = {
    { CIGS_DATA_TABLE_START_ADDRESS,   CIGS_DATA_TABLE_END_ADDRESS,   MISF_CIGS_DATA_TABLE_SIZE },
    { CIGS_PICLOG_START_ADDRESS,       CIGS_PICLOG_END_ADDRESS,       MISF_CIGS_PICLOG_SIZE },
    { CIGS_ENVIRO_START_ADDRESS,       CIGS_ENVIRO_END_ADDRESS,       MISF_CIGS_ENVIRO_SIZE },
    { CIGS_IV1_HEADER_START_ADDRESS,   CIGS_IV1_HEADER_END_ADDRESS,   MISF_CIGS_IV1_HEADER_SIZE },
    { CIGS_IV1_DATA_START_ADDRESS,     CIGS_IV1_DATA_END_ADDRESS,     MISF_CIGS_IV1_DATA_SIZE },
    { CIGS_IV2_HEADER_START_ADDRESS,   CIGS_IV2_HEADER_END_ADDRESS,   MISF_CIGS_IV2_HEADER_SIZE },
    { CIGS_IV2_DATA_START_ADDRESS,     CIGS_IV2_DATA_END_ADDRESS,     MISF_CIGS_IV2_DATA_SIZE }
};





void smf_data_table_init()
{
    fprintf(PC, "SMF Data Table Initialize\r\n");
    for ( unsigned int32 address = CIGS_DATA_TABLE_START_ADDRESS; address < CIGS_IV2_DATA_END_ADDRESS; address += SUBSECTOR_SIZE )
    {
        subsector_4kByte_erase(smf, address);
    }
    smf_data_table_t smf_data_table = {0};
    smf_data_table.fields.crc = calc_crc8(smf_data_table.bytes, PACKET_SIZE - 1); // CRCを計算して初期化    
    write_data_bytes(smf, CIGS_DATA_TABLE_START_ADDRESS, smf_data_table.bytes, PACKET_SIZE);
}

void read_smf_header(smf_data_table_t *smf_data_table)
{
    int8 retry_count;

    for (retry_count = 0; retry_count < CRC_RETRY_COUNT; retry_count++)
    {
        // ヘッダを読み出し
        read_data_bytes(smf, CIGS_DATA_TABLE_START_ADDRESS, smf_data_table->bytes, PACKET_SIZE);
        print_smf_counter_status(smf_data_table);
        // CRC検証
        if (is_crc_valid(smf_data_table->bytes, PACKET_SIZE-1))
        {
            printf("CRC verification passed on attempt %d\r\n", retry_count + 1);
            return; // 成功したら終了
        }
        // NGなら少し待って再試行
        delay_ms(5);
    }

    // ここまで来たら失敗
    printf("CRC verification failed after %d attempts\r\n", CRC_RETRY_COUNT);

    // 読み込み失敗時は初期化しておく
    smf_data_table_init();
    read_data_bytes(smf, CIGS_DATA_TABLE_START_ADDRESS, smf_data_table->bytes, PACKET_SIZE);

    return;
}


void smf_write_header(smf_data_table_t *smf_data_table)
{
    int1 crc_valid = 0;
    int8 readdata[PACKET_SIZE];
    smf_data_table->fields.crc = calc_crc8(smf_data_table->bytes, PACKET_SIZE - 1); // CRCを更新

    for (int8 retry_count = 0; retry_count < CRC_RETRY_COUNT; retry_count++)
    {
        subsector_4kByte_erase(smf, CIGS_DATA_TABLE_START_ADDRESS);
        write_data_bytes(smf, CIGS_DATA_TABLE_START_ADDRESS, smf_data_table->bytes, PACKET_SIZE); // ヘッダーを書き込み
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

void print_smf_counter_status(smf_data_table_t *smf_data_table)
{
    fprintf(PC, "\r\n[SMF Counter Status]\r\n");
    fprintf(PC, "PICLOG     : Used=%lu, Uncopied=%u\r\n", smf_data_table->fields.headers[CIGS_PICLOG_DATA].used_size, smf_data_table->fields.headers[CIGS_PICLOG_DATA].loop_counter);
    fprintf(PC, "ENVIRO     : Used=%lu, Uncopied=%u\r\n", smf_data_table->fields.headers[CIGS_ENVIRO_DATA].used_size, smf_data_table->fields.headers[CIGS_ENVIRO_DATA].loop_counter);
    fprintf(PC, "IV1_HEADER : Used=%lu, Uncopied=%u\r\n", smf_data_table->fields.headers[CIGS_IV1_HEADER].used_size, smf_data_table->fields.headers[CIGS_IV1_HEADER].loop_counter);
    fprintf(PC, "IV1_DATA   : Used=%lu, Uncopied=%u\r\n", smf_data_table->fields.headers[CIGS_IV1_DATA].used_size, smf_data_table->fields.headers[CIGS_IV1_DATA].loop_counter);
    fprintf(PC, "IV2_HEADER : Used=%lu, Uncopied=%u\r\n", smf_data_table->fields.headers[CIGS_IV2_HEADER].used_size, smf_data_table->fields.headers[CIGS_IV2_HEADER].loop_counter);
    fprintf(PC, "IV2_DATA   : Used=%lu, Uncopied=%u\r\n", smf_data_table->fields.headers[CIGS_IV2_DATA].used_size, smf_data_table->fields.headers[CIGS_IV2_DATA].loop_counter);
    fprintf(PC, "------------------------------\r\n\r\n");
}

void smf_write(FlashOperationStruct *smf_data_ptr)
{
    fprintf(PC, "\r\n_______________________________\r\n");
    fprintf(PC, "_________Start copy_data_________\r\n");
    
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


    smf_data_table_t smf_data_table;
    read_smf_header(&smf_data_table);


    unsigned int32 write_src;
    unsigned int32 write_size;

    //アドレスと自動更新
    if (smf_data_ptr->source_type == SOURCE_MISF_UNCOPIED )
    {
        write_src = MISF_ADDRESS_TABLE[smf_data_ptr->mission_id].start + misf_counter_table[smf_data_ptr->mission_id].used_counter - misf_counter_table[smf_data_ptr->mission_id].uncopied_counter;
        write_size = misf_counter_table[smf_data_ptr->mission_id].uncopied_counter;
    }else if(smf_data_ptr->source_type == SOURCE_MISF_MANUAL)
    {
        write_src = smf_data_ptr->misf_start_addr;
        write_size = smf_data_ptr->misf_size;
    }

    unsigned int32 smf_address_start = SMF_ADDRESS_TABLE[smf_data_ptr->mission_id].start;
    unsigned int32 smf_address_end   = SMF_ADDRESS_TABLE[smf_data_ptr->mission_id].end;
    unsigned int32 misf_address_start = MISF_ADDRESS_TABLE[smf_data_ptr->mission_id].start;
    unsigned int32 misf_address_end   = MISF_ADDRESS_TABLE[smf_data_ptr->mission_id].end;



    // Print SMF ADDRESS
    fprintf(PC,"[SMF ADDRESS]\r\n");
    fprintf(PC, "\t------------\t-------------\t-------------\r\n");
    fprintf(PC, "\tSTART\t0x%08LX\r\n", smf_address_start);
    fprintf(PC, "\tEND\t0x%08LX\r\n", smf_address_end);
    fprintf(PC, "\tUSED\t0x%08LX\r\n", smf_data_table.fields.headers[smf_data_ptr->mission_id].used_size);
    fprintf(PC, "\tNEXT\t0x%08LX\r\n", smf_address_start + smf_data_table.fields.headers[smf_data_ptr->mission_id].used_size);

    // Print MISF ADDRESS
    fprintf(PC,"[MISF ADDRESS]\r\n");
    fprintf(PC, "\t------------\t-------------\t-------------\r\n");
    fprintf(PC, "\tSTART\t0x%08LX\r\n", misf_address_start);
    fprintf(PC, "\tEND\t0x%08LX\r\n", misf_address_end);
    fprintf(PC, "\tSRC\t0x%08LX\r\n", write_src);
    fprintf(PC, "\tSIZE\t0x%08LX\r\n", write_size);
    fprintf(PC, "\t------------\t-------------\t-------------\r\n");


    unsigned int8 buffer[PACKET_SIZE];
    unsigned int32 smf_write_address;
    unsigned int32 misf_read_address;

    while (write_size > 0)
    {
        fprintf(PC, "Remaining size to write: %lu bytes\r\n", write_size);
        memset(buffer, 0x11, PACKET_SIZE);
        smf_write_address = smf_address_start + smf_data_table.fields.headers[smf_data_ptr->mission_id].used_size;
        misf_read_address = write_src;

        read_data_bytes(mis_fm, misf_read_address, buffer, PACKET_SIZE);
        fprintf(PC, "MISF DATA\r\n");
        for (unsigned int32 j = 0; j < PACKET_SIZE; j++) {
            fprintf(PC, "%02X ", buffer[j]);
        }
        fprintf(PC, "\r\n");

        write_data_bytes(smf, smf_write_address, buffer, PACKET_SIZE);
        smf_data_table.fields.headers[smf_data_ptr->mission_id].used_size += PACKET_SIZE;
        misf_counter_table[smf_data_ptr->mission_id].uncopied_counter -= PACKET_SIZE;
        
        write_src += PACKET_SIZE;
        write_size -= PACKET_SIZE;
    }


    // write size area
    smf_write_header(&smf_data_table);
    
    fprintf(PC, "\r\n_________End copy_data__________\r\n");
    fprintf(PC, "_______________________________\r\n\r\n");
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
            return &param.iv1_data;
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
        misf_update_address_area();
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
        misf_update_address_area();
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
