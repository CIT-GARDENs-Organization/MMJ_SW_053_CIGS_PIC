#include "ad7490_driver.h"                          // 同じデバイスフォルダのヘッダー
#include "../hal/mmj_cigs_config.h"             // システム設定
#include "../lib/device/ad7490.h"               // ADCデバイスライブラリ


void ad7490_init()
{
    fprintf(PC, "AD7490 Initialize\r\n");
    output_high(ADC_CS); 
    delay_ms(10); 
    unsigned int16 readdata = ad7490_read(ADC_CIGS2_CURR);
    delay_ms(1);
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
    // fprintf(PC, "\t[ADC] Command: %04LX\r\n", cmd);
    output_low(ADC_CS); 
    delay_us(10); 
    spi_xfer(ADC_STREAM, cmd); 
    #ifdef AD7490_DEBUG
       fprintf(PC,"\t[ADC] <<< %04LX\r\n", cmd);
    #endif
    output_high(ADC_CS); 
    delay_us(10);

    // Read the ADC data
    output_low(ADC_CS); 
    delay_us(10);
    unsigned int16 ans = spi_xfer(ADC_STREAM, 0x0000);
    output_high(ADC_CS); 
    unsigned int16 readdata = ans & 0x0FFF; //Conver LSB <--> MSB
    
    #ifdef AD7490_DEBUG
        fprintf(PC,"\t[ADC] >>> %04LX\r\n", readdata);
    #endif
    return readdata;
}
// End of file
