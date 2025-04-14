#include <mmj_cigs_pic_func.h>

void adc_setup()
{
    /*********setup ADC*****************/
    setup_oscillator(OSC_16MHZ);   //oscillator speed (crystal)
	setup_adc_ports(sAN0 | sAN1 | sAN2 | sAN3 | sAN4 | sAN5, VSS_VDD);
	setup_adc(ADC_CLOCK_DIV_64);

    /********setup DAC*****************/
    setup_dac(DAC_VSS_VDD|DAC_OUTPUT1);                                        //DAC output at pin DAC output1 from 0-VDD volts 
    setup_dac3(DAC_VSS_VDD|DAC_OUTPUT1);                                          //DAC output at pin DAC output1 from 0-VDD volts 
    dac_write3(20);                                                               //output analog voltage for biasing of current sensor
}

void sweep(unsigned int8 parameter)
{
    measurement_step = parameter; // Get the measurement step from the parameter array
    
    // read temperature
    set_adc_channel(3);
    measured_temp_top = read_adc(ADC_START_AND_READ); // read voltage at adc pin
    set_adc_channel(3);
    measured_temp_bot = read_adc(ADC_START_AND_READ); // read voltage at adc pin

    // read PD value
    set_adc_channel(3);
    measured_pd_start = read_adc(ADC_START_AND_READ); // read voltage at adc pin
    
    // read timestamp
    measured_time = timer0_get_time(); // read time from timer0

    // CIGS value
    for (unsigned count = 0; count < measurement_step; count++)
    {    
        // set DAC value
        dac_write(count);
        delay_ms(1); // wait for the DAC to stabilize

        // read CIGS value
        set_adc_channel(3);
        data_buffer[count*2] = read_adc(ADC_START_AND_READ); // read voltage at adc pin
        set_adc_channel(3);
        data_buffer[count*2+1] = read_adc(ADC_START_AND_READ); // read voltage at adc pin
    }

    // read PD value
    set_adc_channel(3);
    measured_pd_end = read_adc(ADC_START_AND_READ); // read voltage at adc pin

    convert_cigs_data();
    convert_header_data();    
}

void write_data_to_flash()
{
    
}

/*
void convert_cigs_data(unsigned int8 measurement_step)
{
    // Save CIGS data to memory
    for (int i = 0; i < measurement_step; i++)
    {
        cigs_data[i*3]      = data_buffer[i*2]& 0xFF;
        cigs_data[i*3+1]    = ((data_buffer[i*2+1] & 0x0F) << 4) | ((data_buffer[i*2] >> 8) & 0x0F);
        cigs_data[i*3+2]    = data_buffer[i*2+1] >> 4 & 0xFF;
    }

}

void convert_header_data
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
*/
