#include "../mmj_cigs_mode.h"
#include "../mmj_cigs_func.h"
#include "../../lib/smf_queue.h"
#include "../../lib/mt25q.h"
#include "../../lib/mission_tools.h"
#include "../../lib/ad7490.h"
#include "../mmj_cigs_flash.h"
#include "../mmj_cigs_piclog.h"

// ___________________ Mode Functions ______________________
void mode_dummy(unsigned int8 uplinkcmd[])
{
   fprintf(PC, "Start MODE DUMMY\r\n");

   DUMMY_CMD dummy_cmd;
   dummy_cmd.id = uplinkcmd[0]; // Get the command ID from the uplink command
   dummy_cmd.param1 = 
      ((unsigned int32)uplinkcmd[1] << 24) |
      ((unsigned int32)uplinkcmd[2] << 16) |
      ((unsigned int32)uplinkcmd[3] << 8)  |
      ((unsigned int32)uplinkcmd[4]);
   dummy_cmd.param2 = 
      ((unsigned int16)uplinkcmd[5] << 8) |
      ((unsigned int16)uplinkcmd[6]);
   fprintf(PC, "\tMODE     : %02X\r\n", dummy_cmd.id);
   fprintf(PC, "\tParam1   : 0x%08LX\r\n", dummy_cmd.param1);
   fprintf(PC, "\tParam2   : 0x%04LX\r\n", dummy_cmd.param2);

   piclog_make(dummy_cmd.id, PICLOG_PARAM_START); // Log the command execution

   // This is a dummy function for testing purposes
   // You can add your own code here
   piclog_make(dummy_cmd.id, PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End MODE DUMMY\r\n");
}



// _________________ Oparation Mode ______________________
#Separate
void mode_measure(unsigned int8 parameter[])
{
   fprintf(PC, "Start MODE MEASURE\r\n");


   unsigned int8 sweep_step = parameter[0]; // Get the measurement step from the parameter array
   piclog_make(parameter[0], PICLOG_PARAM_START); // Log the command execution
   //adc_init();
   sweep(parameter);
   
   
}

#Separate
void mode_measure_voltage()
{
   fprintf(PC, "Start MODE MEASURE Voltage\r\n");
   while(TRUE)
   {
      unsigned int16 cigs_value;
      set_adc_channel(CIGS_VOLT);
      cigs_value = read_adc(ADC_START_AND_READ); // read voltage at adc pin
      //fprintf(PC, "cigs_value: %04X\r\n", cigs_value);
      delay_ms(1000); // wait for the ADC to stabilize
   }
}

#Separate
void mode_copy_SMF()
{
   /*
   unsigned int8 smf_data[SMF_DATA_SIZE] = {0x00};
   unsigned int32 src = 0x00000000;
   unsigned int32 dest = 0x00000000;
   unsigned int32 size = 0x00000000;
   for (int i = 0; i < SMF_DATA_SIZE; i++)
   {
      smf_data[i] = read_data_byte_spi_id(src + i);
      fprintf(PC, "smf_data[%d]: %X\r\n", i, smf_data[i]);
   }
   enqueue_smf_data(src, dest, size);
   fprintf(PC, "enqueue_smf_data\r\n");
   */
}

#Separate
void mode_iv_meas_adc()
{

   fprintf(PC, "Start SWEEP\r\n");
   unsigned int8 measurement_step = 100; // Get the measurement step from the parameter array
   fprintf(PC, "\tSweep step : %u\r\n", measurement_step);
   output_high(CONNECT_CIGS);

   // read temperature
   set_adc_channel(TEMP_TOP);
   delay_us(10); // wait for the ADC to stabilize
   measured_temp_top = ad7490_read(AD7490_VIN0); // read voltage at adc pin
   set_adc_channel(TEMP_BOT);
   delay_us(10); // wait for the ADC to stabilize
   measured_temp_bot = ad7490_read(AD7490_VIN0); // read voltage at adc pin
   unsigned int32 current;   
    // read PD value
    set_adc_channel(PD);
    delay_us(10); // wait for the ADC to stabilize
    // = ad7490_read(AD7490_VIN0); // voltage at adc pin
    
    // read timestamp
    //measured_time = timer0_get_time(); // read time from timer0

   // CIGS value
   for (unsigned int16 count = 0; count < measurement_step; count++)
   {    
      // set DAC value
      dac_write(count);
      delay_us(100); // wait for the DAC to stabilize

      // read CIGS voltage and current
      set_adc_channel(CIGS_VOLT);       
      delay_us(10); // wait for the ADC to stabilize
      data_buffer[count*2] =ad7490_read(AD7490_VIN0);  // read voltage at adc pin
      
      set_adc_channel(CIGS_CURR);
      current = 0; // reset current value
      for(int k=0; k<10; k++)
      {
                                          //  routing nth channel to adc//verYOMOGI 20220214update,byUCHIDA
            delay_us(10);
            current = current + ad7490_read(AD7490_VIN0); 
      }    
      
      current = current / 10; // average the current value

      delay_us(10); // wait for the ADC to stabilize
      data_buffer[count*2+1] = current; // store the current value in the buffer
      //data_buffer[count*2+1] = read_adc(ADC_START_AND_READ); // read voltage at adc pin
   }

   fprintf(PC, "END SWEEP\r\n");
   // read PD value
   set_adc_channel(PD);
   //measured_pd_end = read_adc(ADC_START_AND_READ); // read voltage at adc pin

   //convert_cigs_data(measurement_step);
   //convert_header_data();    
   output_low(CONNECT_CIGS);
}









//___________________ Debug Mode _______________
#Separate
void mode_test_temp()
{
   while (TRUE)
   {   
      set_adc_channel(TEMP_TOP);
      unsigned int16 temp_top_value = read_adc(ADC_START_AND_READ); // read voltage at adc pin
      set_adc_channel(TEMP_BOT);
      unsigned int16 temp_bot_value = read_adc(ADC_START_AND_READ); // read voltage at adc pin
      fprintf(PC, "temp_top: %X\r\n", temp_top_value);
      fprintf(PC, "temp_bot: %X\r\n", temp_bot_value);
      delay_ms(1000);
   }
}

#Separate
void mode_test_PD()
{
   while (TRUE)
   {
      unsigned int16 pd_value;
      set_adc_channel(PD);
      pd_value = read_adc(ADC_START_AND_READ); // read voltage at adc pin
      fprintf(PC, "pd_value: %X\r\n", pd_value);
      delay_ms(1000);
   }
}

#Separate
void mode_test_CIGS()
{
   unsigned int16 cigs_value = 0;
   set_adc_channel(3);
   cigs_value = read_adc(ADC_START_AND_READ); // read voltage at adc pin
   fprintf(PC, "cigs_value: %X\r\n", cigs_value);
}

#Separate
void mode_test_current()
{
   unsigned int16 current_value = 0;
   set_adc_channel(3);
   current_value = read_adc(ADC_START_AND_READ); // read voltage at adc pin
   fprintf(PC, "current_value: %X\r\n", current_value);
}

#Separate
void mode_test_voltage()
{
   unsigned int16 voltage_value = 0;
   set_adc_channel(3);
   voltage_value = read_adc(ADC_START_AND_READ); // read voltage at adc pin
   fprintf(PC, "voltage_value: %X\r\n", voltage_value);
}

#separate
void mode_test_iv()
{
   unsigned int8 measurement_step = 100; // Get the measurement step from the parameter array
   fprintf(PC, "Start MODE TEST IV\r\n");
   fprintf(PC, "\tSweep step : %u\r\n", measurement_step);
   //unsigned int16 test = 0x9330;
   //fprintf (PC, "Test value: %04LX\r\n", test);
   output_high(EN_MEAS_VOL);
   output_high(CONNECT_CIGS);
   output_low(EN_NPWR); // Enable NPWR
   unsigned int16 readdata;
   setup_dac(DAC_OUTPUT2 | DAC_VSS_VDD);   
   for (unsigned int16 count = 0; count < measurement_step; count++)
   {    
      // set DAC value
      dac_write(count);
      delay_ms(100); // wait for the DAC to stabilize
      readdata = ad7490_readdata(0x8330);  // read voltage at adc pin
      fprintf(PC, "%04LX, ", readdata);
      readdata = ad7490_readdata(0xAF30);  // read voltage at adc pin
      fprintf(PC, "%04LX\r\n", readdata);
   }
}



// ========================== Flash command ============================

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
