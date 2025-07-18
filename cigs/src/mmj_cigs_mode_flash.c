#include "../mmj_cigs_func.h"
#include "../../lib/tool/smf_queue.h"
#include "../../lib/device/mt25q.h"
#include "../mmj_cigs_flash.h"
#include "../mmj_cigs_piclog.h"

// ========================== Flash Command ============================

#Separate
void mode_flash_erase_all(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Erase All\r\n");
   unsigned int8 cmd = parameter[0]; // Get the command ID from the parameter array
   piclog_make(cmd, 0x00); // Log the command execution
   for (unsigned int32 address = ADDRESS_MISF_START; address < ADDRESS_MISF_END; address += SECTOR_64K_BYTE) {
      sector_erase(mis_fm, address); // Erase each sector
   }
   piclog_make(cmd, PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End Flash Erase All\r\n");
}




#Separate
void mode_flash_erase_1sector(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Erase 1 Sector\r\n");
   unsigned int8 cmd = parameter[0]; // Get the command ID from the parameter array
   unsigned int32 sector_address = 
      ((unsigned int32)parameter[1] << 24) |
      ((unsigned int32)parameter[2] << 16) |
      ((unsigned int32)parameter[3] << 8)  |
      ((unsigned int32)parameter[4]);

   fprintf(PC, "\tSector Address: 0x%08LX\r\n", sector_address);
   piclog_make(cmd, PICLOG_PARAM_START); // Log the command execution
   
   sector_erase(mis_fm, sector_address);
   piclog_make(cmd, PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End Flash Erase 1 Sector\r\n");
}

#Separate
void mode_flash_erase_4kbyte_subsector(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Copy 1 Sector\r\n");
   unsigned int8 cmd = parameter[0]; // Get the command ID from the parameter array
   unsigned int32 subsector_address =
      ((unsigned int32)parameter[1] << 24) |
      ((unsigned int32)parameter[2] << 16) |
      ((unsigned int32)parameter[3] << 8)  |
      ((unsigned int32)parameter[4]);
   
   fprintf(PC, "\tSubsector Address: 0x%08LX\r\n", subsector_address);
   piclog_make(cmd, PICLOG_PARAM_START); // Log the command execution
   
   subsector_4kByte_erase(mis_fm, 0x00000000);
   piclog_make(cmd, PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End Flash Copy 1 Sector\r\n");
}

#Separate
void mode_flash_write_demo(unsigned int8 parameter[])
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

      write_data_bytes(mis_fm, current_address, writedata, PACKET_SIZE);
   }

   piclog_make(flash_write_param.id, PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "\r\n");
   fprintf(PC, "End Flash Write Demo\r\n");
}

#Separate
void mode_flash_write_4kbyte_subsecotr(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Write 4kByte Subsector\r\n");
   piclog_make(parameter[0], PICLOG_PARAM_START); // Log the command execution
   flash_setting(mis_fm);
   unsigned int32 write_address = 0x00000000;
   int8 write_data[256] = {0x01, 0x02, 0x03, 0x04}; // Example data
   write_data_bytes(mis_fm, write_address, write_data, 256);
   piclog_make(parameter[0], PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End Flash Write 4kByte Subsector\r\n");
}

#Separate
void mode_flash_read(unsigned int8 uplinkcmd[])
{
   fprintf(PC, "Start Flash Read\r\n");
   piclog_make(uplinkcmd[0], PICLOG_PARAM_START); // Log the command execution
   FLASH_PARAM flash_param = {0};
   // for(unsigned int8 i = 0; i < PARAMETER_LENGTH; i++)
   // {
   //    fprintf(PC, "Parameter[%d]: %02X\r\n", i, parameter[i]);
   // }
   flash_param.id = uplinkcmd[0];
   flash_param.readaddress = 
   ((unsigned int32)uplinkcmd[1] << 24) |
   ((unsigned int32)uplinkcmd[2] << 16) |
   ((unsigned int32)uplinkcmd[3] << 8)  |
   ((unsigned int32)uplinkcmd[4]);

   flash_param.readpacketnum =
    ((unsigned int16)uplinkcmd[7] << 8) |
    ((unsigned int16)uplinkcmd[8]);

   fprintf(PC, "\tMODE     : %02X\r\n", flash_param.id);
   fprintf(PC, "\tAddress  : 0x%08LX\r\n", flash_param.readaddress);
   fprintf(PC, "\tPacketNum: 0x%04LX\r\n", flash_param.readpacketnum);
   
   piclog_make(flash_param.id, 0x00);
   

   unsigned int8 readdata[PACKET_SIZE] = {0x00}; // Initialize read data buffer
   unsigned int32 read_address;
   fprintf(PC, "READ DATA\r\n");

   if(is_connect(mis_fm) == FALSE) {
      fprintf(PC, "Mission Flash is not connected\r\n");
      return;
   }

   for (unsigned int32 packetcount = 0; packetcount < flash_param.readpacketnum; packetcount++){
      read_address = flash_param.readaddress + packetcount * PACKET_SIZE;

      //fprintf(PC, "Packet %lu: Address 0x%08LX\r\n", packetcount, read_address);
      
      read_data_bytes(mis_fm, read_address, readdata, PACKET_SIZE);
      for (unsigned int8 bytecount = 0; bytecount < PACKET_SIZE; bytecount++){
         fprintf(PC,"%02X ",readdata[bytecount]);
      }
      fprintf(PC,"\r\n");
   }
   piclog_make(flash_param.id, PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End Flash Read\r\n");
}

#Separate
void mode_flash_read_address(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Read Address\r\n");
   flash_setting(mis_fm);
   unsigned int32 read_address = 0x00000000;
   int8 read_data[4];
   read_data_bytes(mis_fm, read_address, read_data, 4);
   fprintf(PC, "Read Data: %02X %02X %02X %02X\r\n", read_data[0], read_data[1], read_data[2], read_data[3]);
   fprintf(PC, "End Flash Read Address\r\n");
}

#Separate
void mode_flash_smf_copy(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash SMF Copy\r\n");
   flash_setting(mis_fm);
   flash_setting(smf);
   
   // Example copy operation
   unsigned int32 src_address = 0x00000000;
   unsigned int32 dest_address = 0x00001000; // Example destination address
   int8 read_data[256];
   read_data_bytes(mis_fm, src_address, read_data, 256);
   write_data_bytes(smf, dest_address, read_data, 256);
   
   fprintf(PC, "End Flash SMF Copy\r\n");
}

#Separate
void mode_flash_smf_read(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash SMF Read\r\n");
   int8 read_data[256];
   //read_data_bytes(smf, read_address, read_data, 256);
   fprintf(PC, "Read Data: ");
   for (int i = 0; i < 256; i++)
   {
      fprintf(PC, "%02X ", read_data[i]);
   }
   fprintf(PC, "\r\nEnd Flash SMF Read\r\n");
}

#Separate
void mode_flash_smf_write(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash SMF Write\r\n");
   flash_setting(smf);
   unsigned int32 write_address = 0x00000000;
   int8 write_data[256] = {0x01, 0x02, 0x03, 0x04}; // Example data
   write_data_bytes(smf, write_address, write_data, 256);
   fprintf(PC, "End Flash SMF Write\r\n");
}
  
#Separate
void mode_flash_address_reset(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Address Reset\r\n");
   piclog_make(parameter[0], PICLOG_PARAM_START); // Log the command execution
   unsigned int8 writedata[PACKET_SIZE] = {0x00}; // Initialize write data to zero
   
   write_data_bytes(mis_fm, ADDRESS_MANEGE_START, writedata, PACKET_SIZE);
   misf_init(); // Update the address area after writing


   piclog_make(parameter[0], PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End Flash Address Reset\r\n");
}
