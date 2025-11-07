#include "cigs_mode_flash.h"                      // 同じフォルダのヘッダー
#include "../domain/cigs_iv.h"          // 測定機能
#include "../lib/tool/smf_queue.h"                   // ツールライブラリ
#include "../lib/device/mt25q.h"                     // デバイスライブラリ
#include "../domain/cigs_flash.h"             // ストレージ機能
#include "../domain/cigs_piclog.h"            // ログ機能
#include "../lib/tool/calc_tools.h"

// ========================== MISF Command ============================
void mode_misf_erase_all(unsigned int8 *uplink_cmd_ptr)
{
   printf("Start Flash Erase All\r\n");
   
   flash_mode_param_t param;
   flash_mode_param_t *param_ptr = &param;
   memcpy(param_ptr->bytes, uplink_cmd_ptr, PARAMETER_LENGTH);
   piclog_save(param_ptr->fields.cmd, 0x00);
   
   // Erase all sectors in the MISF flash memory
   for (int32 address = MISF_START; address < MISF_END; address += SECTOR_64K_BYTE) {
      sector_erase(mis_fm, address); // Erase each sector
   }

   piclog_save(param_ptr->fields.cmd, PICLOG_PARAM_END); // Log the end of the command execution
   printf("End Flash Erase All\r\n");
}

void mode_misf_erase_1sector(unsigned int8 *uplink_cmd_ptr)
{
   printf("Start Flash Erase 1 Sector\r\n");
   flash_mode_param_t param;
   flash_mode_param_t *param_ptr = &param;
   memcpy(param_ptr->bytes, uplink_cmd_ptr, PARAMETER_LENGTH);
   piclog_save(param_ptr->fields.cmd, PICLOG_PARAM_START); // Log the command execution

   int32 sector_address = param_ptr->fields.address;

   printf("\tSector Address: 0x%08LX\r\n", sector_address);
   printf("\tErase Size   : %02LX KByte\r\n", param_ptr->fields.sector_size);

   for (int i = 0; i < param_ptr->fields.sector_size; i++) {
      sector_erase(mis_fm, sector_address + (i * SECTOR_64K_BYTE));
   }

   piclog_save(param_ptr->fields.cmd, PICLOG_PARAM_END); // Log the end of the command execution
   printf("End Flash Erase 1 Sector\r\n");
}

void mode_misf_erase_4kbyte_subsector(unsigned int8 *uplink_cmd_ptr)
{
   printf("Start Flash Erase 4kByte Subsector\r\n");
   flash_mode_param_t param;
   flash_mode_param_t *param_ptr = &param;
   memcpy(param_ptr->bytes, uplink_cmd_ptr, PARAMETER_LENGTH);
   int32 subsector_address = param_ptr->fields.address;
   piclog_save(param_ptr->fields.cmd, PICLOG_PARAM_START);


   printf("\tErase Address: 0x%08LX\r\n", subsector_address);
   printf("\tErase Size   : 0x%08LX\r\n", param_ptr->fields.sector_size);

   for (int i = 0; i < param_ptr->fields.sector_size; i += SUBSECTOR_4K_BYTE) {
      subsector_4kByte_erase(mis_fm, subsector_address + (i * SUBSECTOR_4K_BYTE));
   }

   piclog_save(param_ptr->fields.cmd, PICLOG_PARAM_END);
   printf("End Flash Erase 4kByte Subsector\r\n");
}

void mode_misf_erase_64kbyte_subsector(unsigned int8 *uplink_cmd_ptr)
{
   fprintf(PC, "Start Flash Erase 64kByte Subsector\r\n");
   flash_mode_param_t param;
   flash_mode_param_t *param_ptr = &param;
   memcpy(param_ptr->bytes, uplink_cmd_ptr, PARAMETER_LENGTH);
   piclog_save(param_ptr->fields.cmd, PICLOG_PARAM_START); // Log the command execution

   unsigned int32 subsector_address = param_ptr->fields.address;
   
   fprintf(PC, "\tSubsector Address: 0x%08LX\r\n", subsector_address);
   fprintf(PC, "\tErase Size      : 0x%08LX\r\n", param_ptr->fields.sector_size);

   for (unsigned int32 addr = 0; addr < param_ptr->fields.sector_size; addr += SECTOR_64K_BYTE) {
      sector_erase(mis_fm, subsector_address + addr);
   }

   piclog_save(param_ptr->fields.cmd, PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End Flash Erase 64kByte Subsector\r\n");
}


void mode_misf_write_4kbyte_subsector(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Write 4kByte Subsector\r\n");
   piclog_save(parameter[0], PICLOG_PARAM_START); // Log the command execution
   flash_setting(mis_fm);
   unsigned int32 write_address = 0x00000000;
   //int8 write_data[256] = {0x01, 0x02, 0x03, 0x04}; // Example data
   //write_data_bytes(mis_fm, write_address, write_data, 256);
   piclog_save(parameter[0], PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End Flash Write 4kByte Subsector\r\n");
}


void mode_misf_read(unsigned int8 *uplinkcmd_ptr)
{
   fprintf(PC, "Start Flash Read\r\n");
   flash_mode_param_t flash_param;
   flash_mode_param_t *flash_param_ptr = &flash_param;
   memcpy(flash_param_ptr->bytes, uplinkcmd_ptr, PARAMETER_LENGTH);
   
   piclog_save(flash_param_ptr->fields.cmd, PICLOG_PARAM_START); // Log the command execution
   fprintf(PC, "\tAddress  : 0x%08LX\r\n", flash_param_ptr->fields.address);
   fprintf(PC, "\tPacketNum: 0x%04LX\r\n", flash_param_ptr->fields.copy_packet_size);

   
   unsigned int8 readdata[PACKET_SIZE] = {0x00}; // Initialize read data buffer
   unsigned int32 read_address = flash_param_ptr->fields.address;
   fprintf(PC, "ADDRESS  :\r\n");

   if(is_connect(mis_fm) == FALSE) {
      fprintf(PC, "Mission Flash is not connected\r\n");
      piclog_save(flash_param_ptr->fields.cmd, PICLOG_PARAM_END);
      return; // FIX: 接続失敗時終了
   }

   for (unsigned int32 packetcount = 0; packetcount < flash_param_ptr->fields.copy_packet_size; packetcount++){
      read_address = flash_param_ptr->fields.address + packetcount * PACKET_SIZE;
      // fprintf(PC, "ADDRESS 0x%08LX DATA ",read_address);
      // 終端チェック
      if(read_address > MISF_END){
         fprintf(PC, "[FLASH] Read address 0x%08LX exceeds device end 0x%08LX -> stop\r\n", read_address, (unsigned int32)MISF_END);
         break;
      }
      if(read_address + (PACKET_SIZE - 1) > MISF_END){
         unsigned int32 remain = (MISF_END - read_address) + 1;
         fprintf(PC, "[FLASH] End reached. Partial read %lu bytes.\r\n", remain);
         read_data_bytes(mis_fm, read_address, readdata, remain);
         for (unsigned int8 bytecount = 0; bytecount < remain; bytecount++)
            fprintf(PC, "%02X ", readdata[bytecount]);
         fprintf(PC, "\r\n");
         break;
      }
      read_data_bytes(mis_fm, read_address, readdata, PACKET_SIZE);
      fprintf(PC, "%08LX : ",read_address);
      for (unsigned int8 bytecount = 0; bytecount < PACKET_SIZE; bytecount++){
         fprintf(PC,"%02X ",readdata[bytecount]);
      }
      fprintf(PC,"\r\n");
   }
   piclog_save(flash_param_ptr->fields.cmd, PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End Flash Read\r\n");
}


void mode_misf_erase_and_reset(unsigned int8 *uplinkcmd_ptr)
{
   fprintf(PC, "Start Flash Erase and Reset\r\n");
   piclog_save(uplinkcmd_ptr[0], PICLOG_PARAM_START); // Log the command execution

   mode_misf_erase_all(uplinkcmd_ptr); // Erase all flash memory
   mode_misf_address_reset(uplinkcmd_ptr); // Reset the address area

   fprintf(PC, "End Flash Erase and Reset\r\n");
}


// ========================== SMF Command ============================
void mode_smf_copy(unsigned int8 *parameter)
{
   fprintf(PC, "Start Flash SMF Copy\r\n");
   flash_mode_param_t param;
   flash_mode_param_t *param_ptr = &param;
   memcpy(param_ptr->bytes, parameter, PARAMETER_LENGTH);
   piclog_save(param_ptr->fields.cmd, PICLOG_PARAM_START); // Log the command execution

   // パラメータ表示
   fprintf(PC, "\tAddress  : 0x%08LX\r\n", param_ptr->fields.address);
   fprintf(PC, "\tPacketNum: 0x%04LX\r\n", param_ptr->fields.copy_packet_size);

   // 
   int8 mission_id = parameter[0];
   
   // 未コピーデータの自動転送をキューに追加
   //enqueue_auto_transfer(mission_id);
   
   // Example copy operation with integration system
   int32 src_address = 0x00000000;
   int32 dest_address = 0x00001000; // Example destination address
   int8 read_data[256];
   read_data_bytes(mis_fm, src_address, read_data, 256);
   write_data_bytes(smf, dest_address, read_data, 256);
   
   // 転送完了後のカウンタ更新
   //complete_transfer_and_update_counter(mission_id, 256);
   
   printf("End Flash SMF Copy\r\n");
}


void mode_smf_read(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash SMF Read\r\n");
   unsigned int32 read_address = 
      ((unsigned int32)parameter[1] << 24) |
      ((unsigned int32)parameter[2] << 16) |
      ((unsigned int32)parameter[3] << 8)  |
      ((unsigned int32)parameter[4]);
   unsigned int16 read_packetnum = 
      ((unsigned int16)parameter[6] << 8) |
      ((unsigned int16)parameter[7]);
   
   unsigned int8 read_data[PACKET_SIZE];

   fprintf(PC, "\tAddress  : 0x%08LX\r\n", read_address);
   fprintf(PC, "\tPacketNum: 0x%04LX\r\n", read_packetnum);
   fprintf(PC, "Read Data\r\n");

   while (read_packetnum > 0) {
      read_data_bytes(smf, read_address, read_data, PACKET_SIZE);

      for (int i = 0; i < PACKET_SIZE; i++) {
         fprintf(PC, "%02X ", read_data[i]);
      }
      fprintf(PC, "\r\n");
      read_address += PACKET_SIZE;
      read_packetnum--;
   }
   fprintf(PC, "\r\nEnd Flash SMF Read\r\n");
}


void mode_smf_erase(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash SMF Erase\r\n");
   flash_setting(smf);
   unsigned int32 erase_address = 0x00000000; // Example address
   sector_erase(smf, erase_address); // Erase the sector
   fprintf(PC, "End Flash SMF Erase\r\n");
}  

void mode_misf_address_reset(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Address Reset\r\n");
   piclog_save(parameter[0], PICLOG_PARAM_START); 
   FlashData_t flash_data;
   memset(flash_data.bytes, 0, PACKET_SIZE);
   /*
   for (int i = 0; i < PACKET_SIZE; i++) {
      fprintf(PC, "%02X ", flash_data.bytes[i]);
   }
   */
   fprintf(PC, "\r\n");
   flash_data.packet.crc = calc_crc8(flash_data.bytes, PACKET_SIZE-1);
   for (int i = 0; i < PACKET_SIZE; i++) {
      fprintf(PC, "%02X ", flash_data.bytes[i]);
   }
   fprintf(PC, "\r\n");
   write_data_bytes(mis_fm, MISF_CIGS_DATA_TABLE_START, flash_data.bytes, PACKET_SIZE);
   memset(flash_data.bytes, 0, PACKET_SIZE);
   read_data_bytes(mis_fm, MISF_CIGS_DATA_TABLE_START, flash_data.bytes, PACKET_SIZE);
   flash_data.packet.crc = calc_crc8(flash_data.bytes, PACKET_SIZE-1);
   for (int i = 0; i < PACKET_SIZE; i++) {
      fprintf(PC, "%02X ", flash_data.bytes[i]);
   }
   misf_init(); // Update the address area after writing


   piclog_save(parameter[0], PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End Flash Address Reset\r\n");
}

// ---------- SMF Command Functions ----------
void mode_smf_read_force(int8 parameter[])
{
   int32 address = 
      ((int32)parameter[1] << 24) |
      ((int32)parameter[2] << 16) |
      ((int32)parameter[3] << 8)  |
      ((int32)parameter[4]);
   
   int16 packet_num = 
      ((int16)parameter[7] << 8) |
      ((int16)parameter[8]);
      
   int8 read_data[PACKET_SIZE] = {0x00}; // Initialize read data buffer
   if(!is_connect(smf)) {
      printf("SMF is not connected\r\n");
      return;
   }
   printf("Start Flash SMF Read Force\r\n");
   piclog_save(parameter[0], PICLOG_PARAM_START); // Log the command execution

   // 統合管理システムからキューに追加
   // enqueue_read_data(address, packet_num * PACKET_SIZE);

   printf("\tAddress  : 0x%08LX\r\n", address);
   printf("\tPacketNum: 0x%04LX\r\n", packet_num);
   printf("read data\r\n");
   for (int16 packet_count = 0; packet_count < packet_num; packet_count++)
   {
      int32 current_address = address + (packet_count * PACKET_SIZE);
      read_data_bytes(smf, current_address, read_data, PACKET_SIZE);
      for (int8 byte_count = 0; byte_count < PACKET_SIZE; byte_count++)
      {
         printf("%02X ", read_data[byte_count]);
      }
      printf("\r\n");
   }

   printf("\r\nEnd Flash SMF Read Force\r\n");
   piclog_save(parameter[0], PICLOG_PARAM_END); // Log the end of the command execution
}

void mode_smf_erase_force(int8 parameter[])
{
   printf("Start SMF Erase All\r\n");
   int8 cmd = parameter[0]; // Get the command ID from the parameter array
   piclog_save(cmd, 0x00); // Log the command execution
   

   
   for (int32 address = CIGS_DATA_TABLE_START_ADDRESS; address < CIGS_IV2_DATA_END_ADDRESS; address += SECTOR_64K_BYTE) {
      sector_erase(smf, address); // Erase each sector
   }
   piclog_save(cmd, PICLOG_PARAM_END); // Log the end of the command execution
   printf("End SMF Erase All\r\n");
}

void mode_smf_address_reset(int8 parameter[])
{
   printf("Start SMF Reset\r\n");
   piclog_save(parameter[0], PICLOG_PARAM_START);
   int8 writedata[PACKET_SIZE] = {0x00}; 
   for (int32 address = CIGS_DATA_TABLE_START_ADDRESS; address < CIGS_IV2_DATA_END_ADDRESS; address += SECTOR_64K_BYTE) {
      sector_erase(smf, address); // Erase each sector
   }
   smf_data_table_init(); // Update the address area after writing

   piclog_save(parameter[0], PICLOG_PARAM_END); // Log the end of the command execution
   printf("End SMF Reset\r\n");
}
