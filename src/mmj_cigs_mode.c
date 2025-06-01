#include "include/mmj_cigs_func.h"
#include "./lib/smf_queue.h"
#include "./lib/flash.h"
#include "./lib/mission_tools.h"

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

// Flash command 
#Separate
void mode_flash_erase_all()
{
   fprintf(PC, "Start Flash Erase All\r\n");
   flash_setting(mis_fm);
   sector_erase(mis_fm, 0x00000000);
   fprintf(PC, "End Flash Erase All\r\n");
}

#Separate
void mode_flash_erase_1sector()
{
   fprintf(PC, "Start Flash Erase 1 Sector\r\n");
   flash_setting(mis_fm);
   sector_erase(mis_fm, 0x00000000);
   fprintf(PC, "End Flash Erase 1 Sector\r\n");
}

#Separate
void mode_flash_copy_1sector()
{
   fprintf(PC, "Start Flash Copy 1 Sector\r\n");
   flash_setting(mis_fm);
   sector_erase(mis_fm, 0x00000000);
   fprintf(PC, "End Flash Copy 1 Sector\r\n");
}
