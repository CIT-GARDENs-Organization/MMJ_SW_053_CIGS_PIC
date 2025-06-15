#include "../mmj_cigs_config.h"
#include "../mmj_cigs_piclog.h"


/*
struct AD7490_STREAM ad7490_stream = {
    ADC_STREAM,    // spi_stream
    ADC_CS  // cs_pin
};
*/






void io_init()
{
    fprintf(PC, "IO Initialize\r\n");
    output_low(CONNECT_CIGS);
    output_low(EN_NPWR);
    fprintf(PC, "\tComplete\r\n");
}

void adc_init()
{
   fprintf(PC, "ADC Initialize\r\n");
    //setup ADC
   setup_oscillator(OSC_16MHZ);   //oscillator speed (crystal)
   setup_adc_ports(CIGS_VOLT | CIGS_CURR | TEMP_BOT | TEMP_TOP | PD , DAC_VSS_FVR);
   setup_adc(ADC_CLOCK_DIV_64);

   //setup DAC
   setup_dac(DAC_OUTPUT2 | DAC_VSS_FVR);                                        //DAC output at pin DAC output1 from 0-VDD volts  
   dac_write(50);                                                               
   fprintf(PC, "\tComplete\r\n");
}

#Separate
void sweep(unsigned int8 parameter)
{

    fprintf(PC, "Start SWEEP\r\n");
    unsigned int8 measurement_step = parameter; // Get the measurement step from the parameter array
    fprintf(PC, "\tSweep step : %u\r\n", measurement_step);
    output_high(CONNECT_CIGS);

    // read temperature
    set_adc_channel(TEMP_TOP);
    delay_us(10); // wait for the ADC to stabilize
    measured_temp_top = read_adc(ADC_START_AND_READ); // read voltage at adc pin
    set_adc_channel(TEMP_BOT);
    delay_us(10); // wait for the ADC to stabilize
    measured_temp_bot = read_adc(ADC_START_AND_READ); // read voltage at adc pin

    // read PD value
    set_adc_channel(PD);
    delay_us(10); // wait for the ADC to stabilize
    measured_pd_start = read_adc(ADC_START_AND_READ); // read voltage at adc pin
    
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
        data_buffer[count*2] = read_adc(ADC_START_AND_READ);  // read voltage at adc pin
        
        set_adc_channel(CIGS_CURR);
        current = 0; // reset current value
        for(int k=0; k<10; k++)
        {
                                            //  routing nth channel to adc//verYOMOGI 20220214update,byUCHIDA
            delay_us(10);
            current = current + read_adc(ADC_START_AND_READ);
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


void convert_cigs_data(unsigned int8 measurement_step)
{
    fprintf(PC, "Logged CIGS data\r\n");
    // unsigned int8 measurement_step = 100;
    // Save CIGS data to memory
    for (unsigned int8 i = 0; i < measurement_step; i++)
    {
        cigs_data[i*3]      = (data_buffer[i*2] >> 4) & 0xFF;
        fprintf(PC, "%X ", cigs_data[i*3]);
        cigs_data[i*3+1]    = (data_buffer[i*2] & 0x0F) << 4 | (data_buffer[i*2+1] >>8 )& 0x0F;
        fprintf(PC, "%X ", cigs_data[i*3+1]);
        cigs_data[i*3+2]    = data_buffer[i*2+1] & 0xFF;
        fprintf(PC, "%X ", cigs_data[i*3+2]);
    }
    /*
    fprintf(PC, "Logging CIGS data\r\n");
    for (unsigned int16 j = 0; j < measurement_step*3; j++)
    {
        fprintf(PC, "%X ", cigs_data[j]);
    }
    */
   fprintf(PC, "\r\n");
}


void convert_header_data()
{    
    // Save CIGS data header
    cigs_data_header[0] = 0xFF; // Header start
    cigs_data_header[1] = 0x0F; // Header start

    // Save measurement start time
    cigs_data_header[2] = measured_time & 0xFF;         // Extract the lower 8 bits
    cigs_data_header[3] = (measured_time >> 8) & 0xFF;  //
    cigs_data_header[4] = (measured_time >> 16) & 0xFF;


    // Save measurement PD value & Mode Data 
    cigs_data_header[5] = measured_pd_start & 0xFF;
    cigs_data_header[6] = ((measured_pd_start & 0x0F) << 4) | ((measured_pd_end >> 8) & 0x0F);
    cigs_data_header[7] = measured_pd_end >> 4 & 0xFF;
    
    // Save measurement Temp value
    cigs_data_header[8] = measured_pd_start & 0xFF;
    cigs_data_header[9] = ((measured_pd_start & 0x0F) << 4) | ((measured_pd_end >> 8) & 0x0F);
    cigs_data_header[10] = measured_pd_end >> 4 & 0xFF;

    cigs_data_header[11] = 0x00; // Header end
}

