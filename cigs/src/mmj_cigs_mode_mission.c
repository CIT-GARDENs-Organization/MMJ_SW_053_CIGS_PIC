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

#separate
void mode_test_iv(unsigned int8 *uplinkcmd[])
{
   unsigned int8 measurement_step = uplinkcmd[1]; // Get the measurement step from the parameter array
   fprintf(PC, "Start MODE TEST IV\r\n");
   fprintf(PC, "\tSweep step : %u\r\n", measurement_step);
   output_high(CONNECT_CIGS1);
   output_low(EN_NPWR); // Enable NPWR
   unsigned int16 readdata;

   test_sweep(uplinkcmd); // Call the sweep function with the measurement step

   fprintf(PC, "End MODE TEST IV\r\n");
   output_low(CONNECT_CIGS1);
   output_high(EN_NPWR); // Disable NPWR
}

// _________________ Oparation Mode ______________________
#Separate
void mode_measure(unsigned int8 parameter[])
{
   fprintf(PC, "Start MODE MEASURE\r\n");


   unsigned int8 sweep_step = parameter[0]; // Get the measurement step from the parameter array
   piclog_make(parameter[0], PICLOG_PARAM_START); // Log the command execution
   //adc_init();
   for (unsigned int8 count = 0; count < 40; count++)
   {
      // set DAC value
      //dac_write(count);
      delay_ms(100); // wait for the DAC to stabilize

      // read CIGS voltage and current
      data_buffer[count*2] = ad7490_read(AD7490_VIN0); // read voltage at adc pin
      data_buffer[count*2+1] = ad7490_read(AD7490_VIN1); // read current at adc pin

      // monitoring
      fprintf(PC, "%04LX,%04LX\r\n", data_buffer[count*2], data_buffer[count*2+1]);
   }
   
   
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


