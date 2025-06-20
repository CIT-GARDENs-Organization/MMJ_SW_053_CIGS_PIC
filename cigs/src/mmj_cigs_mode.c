#include "../mmj_cigs_mode.h"
#include "../mmj_cigs_func.h"
#include "../../lib/smf_queue.h"
#include "../../lib/mt25q.h"
#include "../../lib/mission_tools.h"
#include "../../lib/ad7490.h"
#include "../mmj_cigs_flash.h"
#include "../mmj_cigs_piclog.h"
// _________________ Oparation Mode ______________________
#Separate
void mode_measure(unsigned int8 parameter[])
{
   fprintf(PC, "Start MODE MEASURE\r\n");
   unsigned int8 sweep_step = parameter[0]; // Get the measurement step from the parameter array
   
   //adc_init();
   sweep(sweep_step);
   //copy_data();

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
    measured_pd_start = ad7490_read(AD7490_VIN0); // voltage at adc pin
    
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
      measured_pd_end = read_adc(ADC_START_AND_READ); // read voltage at adc pin

      convert_cigs_data(measurement_step);
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





// ========================== Flash command ============================

#Separate
void mode_flash_erase_all(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Erase All\r\n");
   flash_setting(mis_fm);
   sector_erase(mis_fm, 0x00000000);
   fprintf(PC, "End Flash Erase All\r\n");
}

#Separate
void mode_flash_erase_1sector(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Erase 1 Sector\r\n");
   flash_setting(mis_fm);
   sector_erase(mis_fm, 0x00000000);
   fprintf(PC, "End Flash Erase 1 Sector\r\n");
}

#Separate
void mode_flash_erase_4kbyte_subsector(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Copy 1 Sector\r\n");
   flash_setting(mis_fm);
   sector_erase(mis_fm, 0x00000000);
   fprintf(PC, "End Flash Copy 1 Sector\r\n");
}

#Separate
void mode_flash_write_demo(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Write Demo\r\n");
   sector_erase(mis_fm, 0x000000); // Erase the sector before writing
   delay_ms(100); // Wait for the erase operation to complete
   unsigned int32 write_address = 0x000000;
   int8 write_data[4] = {0x01, 0x02, 0x03, 0x04};
   write_data_bytes(mis_fm, write_address, write_data, 4);
   fprintf(PC, "End Flash Write Demo\r\n");
   int8 read_data[4];
   read_data_bytes(mis_fm, write_address, read_data, 4);
   fprintf(PC, "Read Data: %02X %02X %02X %02X\r\n", read_data[0], read_data[1], read_data[2], read_data[3]);
   write_data[0] = 0x05; // Modify the data to be written
   write_data[1] = 0x06;
   write_data[2] = 0x07;
   write_data[3] = 0x08;
   write_data_bytes(mis_fm, write_address+4, write_data, 4);
   read_data_bytes(mis_fm, write_address+4, read_data, 4);
   fprintf(PC, "Read Data: %02X %02X %02X %02X\r\n", read_data[0], read_data[1], read_data[2], read_data[3]);
   fprintf(PC, "End Flash Write Demo\r\n");
}

#Separate
void mode_flash_write_4kbyte_subsecotr(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Write 4kByte Subsector\r\n");
   flash_setting(mis_fm);
   unsigned int32 write_address = 0x00000000;
   int8 write_data[256] = {0x01, 0x02, 0x03, 0x04}; // Example data
   write_data_bytes(mis_fm, write_address, write_data, 256);
   fprintf(PC, "End Flash Write 4kByte Subsector\r\n");
}

#Separate
void mode_flash_read(unsigned int8 parameter[])
{
   fprintf(PC, "Start Flash Read\r\n");
   FLASH_PARAM flash_param = {0};
   // for(unsigned int8 i = 0; i < PARAMETER_LENGTH; i++)
   // {
   //    fprintf(PC, "Parameter[%d]: %02X\r\n", i, parameter[i]);
   // }
   flash_param.id = parameter[0];
   flash_param.readaddress = 
   ((unsigned int32)parameter[1] << 24) |
   ((unsigned int32)parameter[2] << 16) |
   ((unsigned int32)parameter[3] << 8)  |
   ((unsigned int32)parameter[4]);

   flash_param.readpacketnum =
    ((unsigned int16)parameter[7] << 8) |
    ((unsigned int16)parameter[8]);

   fprintf(PC, "\tMODE     : %02X\r\n", flash_param.id);
   fprintf(PC, "\tAddress  : 0x%08LX\r\n", flash_param.readaddress);
   fprintf(PC, "\tPacketNum: 0x%04LX\r\n", flash_param.readpacketnum);
   
   piclog_make(flash_param.id, 0x00);
   

   unsigned int8 readdata[PACKET_SIZE];
   fprintf(PC, "READ DATA\r\n");
   for (unsigned int8 packetcount = 0; packetcount < flash_param.readpacketnum; packetcount++){
      read_data_bytes(mis_fm,flash_param.readaddress + packetcount * PACKET_SIZE, readdata, PACKET_SIZE);
      for (unsigned int8 bitcount = 0; bitcount < PACKET_SIZE; bitcount++){
         fprintf(PC,"%02X ",readdata[bitcount]);
      }
      fprintf(PC,"\r\n");
   }
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
   FLASH_DATA_HEADER flash_data_header;
   flash_data_header.fields.misf_meas_loop_counter = 0;
   flash_data_header.fields.misf_piclog_loop_counter = 0;
   flash_data_header.fields.misf_piclog_use_counter = 0;
   flash_data_header.fields.misf_meas_uncopyed_counter = 0;

   write_data_bytes(mis_fm, ADDRESS_MANEGE_START, flash_data_header.bytes, PACKET_SIZE);

   fprintf(PC, "End Flash Address Reset\r\n");
}
