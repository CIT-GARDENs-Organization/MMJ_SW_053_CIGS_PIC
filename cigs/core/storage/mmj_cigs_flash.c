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
    read_data_bytes(mis_fm, MISF_CIGS_DATA_TABLE_END, flash_data.bytes, PACKET_SIZE);

    if (flash_data.packet.crc != calc_crc8(flash_data.bytes, PACKET_SIZE - 1)) {
        fprintf(PC, "\t[MIS FM] CRC error!\r\n");
        return;
    }

    //=== カウンタ情報の反映 ===//
    piclog_data       = *((Flash_t*)&flash_data.packet.payload.logdata.piclog);
    environment_data  = *((Flash_t*)&flash_data.packet.payload.logdata.environment);
    iv_header         = *((Flash_t*)&flash_data.packet.payload.logdata.iv_header);
    iv_data           = *((Flash_t*)&flash_data.packet.payload.logdata.iv_data);
    print_flash_status();
}

FlashData_t make_flash_data_table(void)
{
    FlashData_t flash_data;
    memset(&flash_data, 0, sizeof(flash_data));

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

    for(int i=0; i<4; i++){
        dst_list[i]->used_counter     = src_list[i]->used_counter;
        dst_list[i]->uncopied_counter = src_list[i]->uncopied_counter;
        dst_list[i]->reserve_counter1 = src_list[i]->reserve_counter1;
        dst_list[i]->reserve_counter2 = src_list[i]->reserve_counter2;
    }

    flash_data.packet.crc = calc_crc8(flash_data.bytes, PACKET_SIZE - 1);
    return flash_data;
}

void write_misf_address_area(void)
{
    FlashData_t flash_data = make_flash_data_table();

    write_data_bytes(mis_fm, MISF_CIGS_DATA_TABLE_END, flash_data.bytes, PACKET_SIZE);
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
