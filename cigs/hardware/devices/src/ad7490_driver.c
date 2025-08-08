#include "../ad7490_driver.h"                          // 同じデバイスフォルダのヘッダー
#include "../../../system/mmj_cigs_config.h"             // システム設定
#include "../../../../lib/device/ad7490.h"               // ADCデバイスライブラリ


void ad7490_init()
{
    fprintf(PC, "AD7490 Initialize\r\n");
    output_high(ADC_CS); // Set CS pin high to deselect the ADC
    delay_ms(10); // Wait for the ADC to stabilize
    unsigned int16 readdata = ad7490_read(ADC_CIGS2_CURR); // Read a dummy value to initialize the ADC
    delay_ms(1); // Wait for the ADC to stabilize after reading
    fprintf(PC, "\t%04LX\r\n", readdata);
    if (readdata == 0xFFFF || readdata == 0x0000) {
        fprintf(PC, "\tConnect error!\r\n");
    } else {
        fprintf(PC, "\tConnect successful\r\n");
    }
    fprintf(PC, "\tComplete\r\n");
}

unsigned int16 ad7490_read(int8 channel)
{   
    unsigned int16 cmd = ad7490_make_cmd(channel);
    
    // Send the command to the ADC
    output_low(ADC_CS); // Set CS pin low to select the ADC
    delay_us(10); // Wait for the ADC to stabilize
    spi_xfer(ADC_STREAM, cmd); // Dummy transfer to start communication
    #ifdef AD7490_DEBUG
       fprintf(PC,"\t[ADC] <<< %04LX\r\n", cmd);
    #endif
    delay_us(10); // Wait for the ADC to process the command
    output_high(ADC_CS); // Set CS pin high to end communication
    delay_us(10); // Wait for the ADC to process the command

    // Read the ADC data
    output_low(ADC_CS); 
    delay_us(10); // Wait for the ADC to stabilize
    unsigned int16 ans = spi_xfer(ADC_STREAM, 0x0000); // Read data from ADC
    output_high(ADC_CS); 
    unsigned int16 readdata = ans & 0x0FFF; //Conver LSB <--> MSB
    
    
    #ifdef AD7490_DEBUG
        fprintf(PC,"\t[ADC] >>> %04LX\r\n", readdata);
    #endif
    return readdata;
}
