#include "mmj_cigs_pic_mode.h"

//____________ main mission function _______________

void execute_mission(unsigned int8 *content)
{
//!   // timer update
//!   update_time(content);
//!   fprintf(PC, "\r\nUpdate the time setting\r\n");
//!   fprintf(PC, "\t-> Day: \t%lu\r\n", day);
//!   fprintf(PC, "\t-> Hour: \t%lu\r\n", sec / 3600);
//!   fprintf(PC, "\t-> Minute: \t%lu\r\n", (sec / 60) % 60);
//!   fprintf(PC, "\t-> Second: \t%lu\r\n", sec % 60);
//!   
   // record to executed mission list
   unsigned int8 command_id = content[0];
   unsigned int8 *parameter = &content[1];
   for (int i = 0; i < executed_mission_count; i++)
      if (command_id == executed_mission[i])
      {
         fprintf(PC, "\r\nThis  mission is executed\r\n\t-> 0x%d\r\n", command_id);
         return;
      }
   if (executed_mission_count != EXECUTED_MISSION_SIZE)
      executed_mission[executed_mission_count++] = command_id;
   else
      fprintf(PC, "[WARN]  overflow executed_mission list");
   
   
   // execution mission
   fprintf(PC, "\r\n______________________________\r\n_____Start execute_mission_____\r\n\r\n");
   fprintf(PC, "Command ID: %X\r\n\r\n", command_id);
   switch(command_id)
   {
      case 0x00:
         example_00(parameter);
         break;
      case 0x01:
         example_01(parameter);
         break;
      default:
         fprintf(PC, "\t\t-> Invalid CMD ID!\r\n");
   }
   fprintf(PC, "\r\n______End execute_mission______\r\n_______________________________\r\n");
}

// _________________ missions ______________________

void setup()
{
   /*********setup ADC*****************/
   setup_oscillator(OSC_16MHZ);   //oscillator speed (crystal)
	setup_adc_ports(sAN0 | sAN1 | sAN2 | sAN3 | sAN4 | sAN5, VSS_VDD);
	setup_adc(ADC_CLOCK_DIV_64);

   /********setup DAC*****************/
   setup_dac(DAC_VSS_VDD|DAC_OUTPUT1);                                        //DAC output at pin DAC output1 from 0-VDD volts 
//   setup_vref(VREF_ON|VREF_COMP_DAC_2v048); 
   setup_dac3(DAC_VSS_VDD|DAC_OUTPUT1);                                          //DAC output at pin DAC output1 from 0-VDD volts 
   dac_write3(20);                                                               //output analog voltage for biasing of current sensor
   
}

void mode_measure(unsigned int8 parameter[])
{
   unsined in sweep_step = parameter[0]; // Get the measurement step from the parameter array
   setup();
   sweep(sweep_step);
   copy_data();
}

void mode_copy_SMF()
{
   unsigned int8 smf_data[SMF_DATA_SIZE] = {0x00};
   unsigned int32 src = 0x00000000;
   unsigned int32 dest = 0x00000000;
   unsigned int32 size = 0x00000000;
   for (int i = 0; i < SMF_DATA_SIZE; i++)
   {
      smf_data[i] = read_data_byte_flash128m(src + i);
      fprintf(PC, "smf_data[%d]: %X\r\n", i, smf_data[i]);
   }
   enqueue_smf_data(src, dest, size);
   fprintf(PC, "enqueue_smf_data\r\n");
}


//___________________ test functions _______________
void mode_test_temp()
{
   unsigned int16 temp_top = 0;
   unsigned int16 temp_bot = 0;
   set_adc_channel(3);
   temp_top = read_adc(ADC_START_AND_READ); // read voltage at adc pin
   set_adc_channel(3);
   temp_bot = read_adc(ADC_START_AND_READ); // read voltage at adc pin
   fprintf(PC, "temp_top: %X\r\n", temp_top);
   fprintf(PC, "temp_bot: %X\r\n", temp_bot);
}

void mode_test_PD()
{
   unsigned int16 pd_start = 0;
   unsigned int16 pd_end = 0;
   set_adc_channel(3);
   pd_start = read_adc(ADC_START_AND_READ); // read voltage at adc pin
   set_adc_channel(3);
   pd_end = read_adc(ADC_START_AND_READ); // read voltage at adc pin
   fprintf(PC, "pd_start: %X\r\n", pd_start);
   fprintf(PC, "pd_end: %X\r\n", pd_end);
}

void mode_test_CIGS()
{
   unsigned int16 cigs_value = 0;
   set_adc_channel(3);
   cigs_value = read_adc(ADC_START_AND_READ); // read voltage at adc pin
   fprintf(PC, "cigs_value: %X\r\n", cigs_value);
}

void mode_test_current()
{
   unsigned int16 current_value = 0;
   set_adc_channel(3);
   current_value = read_adc(ADC_START_AND_READ); // read voltage at adc pin
   fprintf(PC, "current_value: %X\r\n", current_value);
}

void mode_test_voltage()
{
   unsigned int16 voltage_value = 0;
   set_adc_channel(3);
   voltage_value = read_adc(ADC_START_AND_READ); // read voltage at adc pin
   fprintf(PC, "voltage_value: %X\r\n", voltage_value);
}
