#include "../mmj_cigs_mode_mission.h"
#include "../mmj_cigs_func.h"
#include "../../lib/tool/smf_queue.h"
#include "../../lib/device/mt25q.h"
#include "../../lib/device/ad7490.h"
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
   output_high(CONNECT_CIGS1);

   // read temperature
   delay_us(10); // wait for the ADC to stabilize
   measured_temp_top = ad7490_read(AD7490_VIN0); // read voltage at adc pin
   delay_us(10); // wait for the ADC to stabilize
   measured_temp_bot = ad7490_read(AD7490_VIN0); // read voltage at adc pin
   unsigned int32 current;   
    // read PD value

    delay_us(10); // wait for the ADC to stabilize
    // = ad7490_read(AD7490_VIN0); // voltage at adc pin
    
    // read timestamp
    //measured_time = timer0_get_time(); // read time from timer0

   // CIGS value
   for (unsigned int16 count = 0; count < measurement_step; count++)
   {    
      // set DAC value
      //dac_write(count);
      delay_us(100); // wait for the DAC to stabilize

      // read CIGS voltage and current
      delay_us(10); // wait for the ADC to stabilize
      data_buffer[count*2] =ad7490_read(AD7490_VIN0);  // read voltage at adc pin
      
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
   //measured_pd_end = read_adc(ADC_START_AND_READ); // read voltage at adc pin

   //convert_cigs_data(measurement_step);
   //convert_header_data();    
   output_low(CONNECT_CIGS1);
}








/*
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
void mode_test_iv(unsigned int8 *uplinkcmd[])
{
   
   unsigned int8 measurement_step = uplinkcmd[1]; // Get the measurement step from the parameter array
   fprintf(PC, "Start MODE TEST IV\r\n");
   fprintf(PC, "\tSweep step : %u\r\n", measurement_step);
   //unsigned int16 test = 0x9330;
   //fprintf (PC, "Test value: %04LX\r\n", test);
   output_high(EN_MEAS_VOL);
   output_high(CONNECT_CIGS1);
   output_low(EN_NPWR); // Enable NPWR
   unsigned int16 readdata;
   //setup_dac(DAC_OUTPUT1 | DAC_VSS_VDD); 
   //delay_ms(1); // wait for the DAC to stabilize  

   sweep(uplinkcmd); // Call the sweep function with the measurement step

   fprintf(PC, "End MODE TEST IV\r\n");
   output_low(EN_MEAS_VOL);
   output_low(CONNECT_CIGS11);
   output_high(EN_NPWR); // Disable NPWR

   fprintf(PC, "Add SMF que\r\n");
   SmfDataStruct data;
   data.mission_type = MEAURE_DATA; // コピーする目的のデータ種別
   data.src = ADDRESS_MISF_MEASUREMENT_START + misf_meas_use_counter - misf_meas_uncopyed_counter; // コピー元のMIS_FMのアドレス
   data.size = misf_meas_uncopyed_counter; // コピーするデータのサイズ
   enqueue_smf_data(&data); // SMFへのデータコピーを実行する
}
*/
