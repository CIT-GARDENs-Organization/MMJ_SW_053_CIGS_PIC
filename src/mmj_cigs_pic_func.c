void adc_setup()
{
    /*********setup ADC*****************/
    setup_oscillator(OSC_16MHZ);   //oscillator speed (crystal)
	setup_adc_ports(sAN0 | sAN1 | sAN2 | sAN3 | sAN4 | sAN5, VSS_VDD);
	setup_adc(ADC_CLOCK_DIV_64);

    /********setup DAC*****************/
    setup_dac(DAC_VSS_VDD|DAC_OUTPUT1);                                        //DAC output at pin DAC output1 from 0-VDD volts 
    //setup_vref(VREF_ON|VREF_COMP_DAC_2v048); 
    setup_dac3(DAC_VSS_VDD|DAC_OUTPUT1);                                          //DAC output at pin DAC output1 from 0-VDD volts 
    dac_write3(20);                                                               //output analog voltage for biasing of current sensor
    


}