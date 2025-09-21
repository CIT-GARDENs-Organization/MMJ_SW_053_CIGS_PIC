#include "mmj_cigs_mode_flash.h"                      // 同じフォルダのヘッダー
#include "../domain/mmj_cigs_iv.h"          // 測定機能
#include "../lib/tool/smf_queue.h"                   // ツールライブラリ
#include "../lib/device/mt25q.h"                     // デバイスライブラリ
#include "../domain/mmj_cigs_flash.h"             // ストレージ機能
#include "../domain/mmj_cigs_piclog.h"            // ログ機能
#include "../lib/tool/calc_tools.h"

// ========================== MISF Command ============================
void mode_misf_erase_all(int8 parameter[])
{
   printf("Start Flash Erase All\r\n");
   int8 cmd = parameter[0]; // Get the command ID from the parameter array
   piclog_make(cmd, 0x00); // Log the command execution
   
   for (int32 address = MISF_START; address < MISF_END; address += SECTOR_64K_BYTE) {
      sector_erase(mis_fm, address); // Erase each sector
   }
   piclog_make(cmd, PICLOG_PARAM_END); // Log the end of the command execution
   printf("End Flash Erase All\r\n");
}

void mode_misf_erase_1sector(int8 parameter[])
{
   printf("Start Flash Erase 1 Sector\r\n");
   int8 cmd = parameter[0]; // Get the command ID from the parameter array
   int32 sector_address = 
      ((int32)parameter[1] << 24) |
      ((int32)parameter[2] << 16) |
      ((int32)parameter[3] << 8)  |
      ((int32)parameter[4]);

   printf("\tSector Address: 0x%08LX\r\n", sector_address);
   piclog_make(cmd, PICLOG_PARAM_START); // Log the command execution
   
   sector_erase(mis_fm, sector_address);
   piclog_make(cmd, PICLOG_PARAM_END); // Log the end of the command execution
   printf("End Flash Erase 1 Sector\r\n");
}

void mode_misf_erase_4kbyte_subsector(int8 parameter[])
{
   printf("Start Flash Erase 4kByte Subsector\r\n");
   int8 cmd = parameter[0];
   int32 subsector_address =
      ((int32)parameter[1] << 24) |
      ((int32)parameter[2] << 16) |
      ((int32)parameter[3] << 8)  |
      ((int32)parameter[4]);
   printf("\tSubsector Address: 0x%08LX\r\n", subsector_address);
   piclog_make(cmd, PICLOG_PARAM_START);
   subsector_4kByte_erase(mis_fm, subsector_address); // FIX: 固定0消去→指定アドレス
   piclog_make(cmd, PICLOG_PARAM_END);
   printf("End Flash Erase 4kByte Subsector\r\n");
}

void mode_misf_erase_64kbyte_subsector(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Erase 64kByte Subsector\r\n");
   unsigned int8 cmd = parameter[0]; // Get the command ID from the parameter array
   unsigned int32 subsector_address =
      ((unsigned int32)parameter[1] << 24) |
      ((unsigned int32)parameter[2] << 16) |
      ((unsigned int32)parameter[3] << 8)  |
      ((unsigned int32)parameter[4]);
   
   fprintf(PC, "\tSubsector Address: 0x%08LX\r\n", subsector_address);
   piclog_make(cmd, PICLOG_PARAM_START); // Log the command execution
   
   //subsector_64kByte_erase(mis_fm, subsector_address);
   piclog_make(cmd, PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End Flash Erase 64kByte Subsector\r\n");
}

void mode_misf_write_demo(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Write Demo\r\n");
   piclog_make(parameter[0], PICLOG_PARAM_START); // Log the command execution

   FLASH_WRITE_PARAM flash_write_param = {0};
   
   flash_write_param.id = parameter[0];
   flash_write_param.writeaddress =
   ((unsigned int32)parameter[1] << 24) |
   ((unsigned int32)parameter[2] << 16) |
   ((unsigned int32)parameter[3] << 8)  |
   ((unsigned int32)parameter[4]);
   flash_write_param.packetnum =
   ((unsigned int16)parameter[7] << 8) |
   ((unsigned int16)parameter[8]);

   fprintf(PC, "\tMODE     : %02X\r\n", flash_write_param.id);
   fprintf(PC, "\tAddress  : 0x%08LX\r\n", flash_write_param.writeaddress);
   fprintf(PC, "\tPacketNum: 0x%04LX\r\n", flash_write_param.packetnum);

   piclog_make(flash_write_param.id, 0x00); // Log the command execution
   
   unsigned int8 writedata[64];
   unsigned int16 p; // packet index
   unsigned int16 base_value;

   fprintf(PC, "Write Data\r\n");
   for (p = 0; p < flash_write_param.packetnum; p++)
   {
      base_value = p * PACKET_SIZE;  // パケット毎のスタート値

      for (unsigned int8 i = 0; i < PACKET_SIZE; i++)
      {
         writedata[i] = (base_value + i) & 0xFF; // 0x00〜0xFFをループ
         fprintf(PC, "%02X ", writedata[i]); // デバッグ用に書き込みデータを表示
      }

      unsigned int32 current_address = flash_write_param.writeaddress + (p * PACKET_SIZE);
      // FLASH 終端チェック (MISF_END は最終アドレス想定: inclusive)
      if(current_address > MISF_END){
         fprintf(PC, "\r\n[FLASH] Write address 0x%08LX exceeds device end 0x%08LX -> abort\r\n", current_address, (unsigned int32)MISF_END);
         piclog_make(flash_write_param.id, PICLOG_PARAM_END);
         break;
      }
      if(current_address + (PACKET_SIZE - 1) > MISF_END){
         unsigned int32 remain = (MISF_END - current_address) + 1; // 書込可能残り
         fprintf(PC, "\r\n[FLASH] Reached end. Partial write %lu bytes (packet truncated).\r\n", remain);
         write_data_bytes(mis_fm, current_address, writedata, (unsigned int16)remain);
         piclog_make(flash_write_param.id, PICLOG_PARAM_END);
         break;
      }
      write_data_bytes(mis_fm, current_address, writedata, PACKET_SIZE);
   }

   piclog_make(flash_write_param.id, PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "\r\n");
   fprintf(PC, "End Flash Write Demo\r\n");
}

void mode_misf_write_4kbyte_subsector(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Write 4kByte Subsector\r\n");
   piclog_make(parameter[0], PICLOG_PARAM_START); // Log the command execution
   flash_setting(mis_fm);
   unsigned int32 write_address = 0x00000000;
   //int8 write_data[256] = {0x01, 0x02, 0x03, 0x04}; // Example data
   //write_data_bytes(mis_fm, write_address, write_data, 256);
   piclog_make(parameter[0], PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End Flash Write 4kByte Subsector\r\n");
}


void mode_misf_read(unsigned int8 *uplinkcmd_ptr)
{
   fprintf(PC, "Start Flash Read\r\n");


   FLASH_PARAM flash_param = {0};
   flash_param.id = uplinkcmd_ptr[0];
   flash_param.readaddress = -
      ((unsigned int32)uplinkcmd_ptr[1] << 24) |
      ((unsigned int32)uplinkcmd_ptr[2] << 16) |
      ((unsigned int32)uplinkcmd_ptr[3] << 8)  |
      ((unsigned int32)uplinkcmd_ptr[4]);
   flash_param.readpacketnum = 
      ((unsigned int16)uplinkcmd_ptr[6] << 8) |
      ((unsigned int16)uplinkcmd_ptr[7]);
      piclog_make(flash_param.id, PICLOG_PARAM_START); // Log the command execution
   fprintf(PC, "\tMODE     : %02X\r\n", flash_param.id);
   fprintf(PC, "\tAddress  : 0x%08LX\r\n", flash_param.readaddress);
   fprintf(PC, "\tPacketNum: 0x%04LX\r\n", flash_param.readpacketnum);
   
   piclog_make(flash_param.id, 0x00);
   

   unsigned int8 readdata[PACKET_SIZE] = {0x00}; // Initialize read data buffer
   unsigned int32 read_address;
   fprintf(PC, "READ DATA\r\n");

   if(is_connect(mis_fm) == FALSE) {
      fprintf(PC, "Mission Flash is not connected\r\n");
      piclog_make(flash_param.id, PICLOG_PARAM_END);
      return; // FIX: 接続失敗時終了
   }

   for (unsigned int32 packetcount = 0; packetcount < flash_param.readpacketnum; packetcount++){
      read_address = flash_param.readaddress + packetcount * PACKET_SIZE;
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
      for (unsigned int8 bytecount = 0; bytecount < PACKET_SIZE; bytecount++){
         fprintf(PC,"%02X ",readdata[bytecount]);
      }
      fprintf(PC,"\r\n");
   }
   piclog_make(flash_param.id, PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End Flash Read\r\n");
}


void mode_misf_read_address(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Read Address\r\n");
   flash_setting(mis_fm);
   unsigned int32 read_address = 0x00000000;
   int8 read_data[4];
   read_data_bytes(mis_fm, read_address, read_data, 4);
   fprintf(PC, "Read Data: %02X %02X %02X %02X\r\n", read_data[0], read_data[1], read_data[2], read_data[3]);
   fprintf(PC, "End Flash Read Address\r\n");
}

void mode_misf_erase_and_reset(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Erase and Reset\r\n");
   piclog_make(parameter[0], PICLOG_PARAM_START); // Log the command execution

   mode_misf_erase_all(parameter); // Erase all flash memory
   mode_misf_address_reset(parameter); // Reset the address area

   fprintf(PC, "End Flash Erase and Reset\r\n");
}
// ========================== SMF Command ============================
void mode_smf_copy(int8 parameter[])
{
   printf("Start Flash SMF Copy\r\n");
   flash_setting(mis_fm);
   flash_setting(smf);
   
   // 統合管理システムを使用したコピー操作
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
   int8 read_data[PACKET_SIZE];
   unsigned int32 address = 0; // TODO: parameter からアドレス取得拡張可
   read_data_bytes(smf, address, read_data, PACKET_SIZE); // FIX: 誤引数順
   fprintf(PC, "Read Data: ");
   for (int i = 0; i < PACKET_SIZE; i++)
      fprintf(PC, "%02X ", read_data[i]);
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
   piclog_make(parameter[0], PICLOG_PARAM_START); 
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


   piclog_make(parameter[0], PICLOG_PARAM_END); // Log the end of the command execution
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
   piclog_make(parameter[0], PICLOG_PARAM_START); // Log the command execution

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
   piclog_make(parameter[0], PICLOG_PARAM_END); // Log the end of the command execution
}

void mode_smf_erase_force(int8 parameter[])
{
   printf("Start SMF Erase All\r\n");
   int8 cmd = parameter[0]; // Get the command ID from the parameter array
   piclog_make(cmd, 0x00); // Log the command execution
   

   
   for (int32 address = CIGS_DATA_TABLE_START_ADDRESS; address < CIGS_PICLOG_END_ADDRESS; address += SECTOR_64K_BYTE) {
      sector_erase(smf, address); // Erase each sector
   }
   piclog_make(cmd, PICLOG_PARAM_END); // Log the end of the command execution
   printf("End SMF Erase All\r\n");
}

void mode_smf_address_reset(int8 parameter[])
{
   printf("Start SMF Reset\r\n");
   piclog_make(parameter[0], PICLOG_PARAM_START);
   int8 writedata[PACKET_SIZE] = {0x00}; 
   for (int32 address = CIGS_DATA_TABLE_START_ADDRESS; address < CIGS_IV2_DATA_END_ADDRESS; address += SECTOR_64K_BYTE) {
      sector_erase(smf, address); // Erase each sector
   }
   smf_data_table_init(); // Update the address area after writing

   piclog_make(parameter[0], PICLOG_PARAM_END); // Log the end of the command execution
   printf("End SMF Reset\r\n");
}
