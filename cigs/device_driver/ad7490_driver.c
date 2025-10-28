#include "ad7490_driver.h"                          // 同じデバイスフォルダのヘッダー
#include "../hal/cigs_config.h"             // システム設定
#include "../lib/device/ad7490.h"               // ADCデバイスライブラリ


#define AD7490_MULTI_CHANNEL_MAX 4

unsigned int16 ad7490_make_single_channel_cmd(unsigned int8 channel)
{
    ad7490_cmd_t cmd;
    
    // Set default values for all fields
    cmd.fields.reserved0 = 0; 
    cmd.fields.reserved1 = 0; 
    cmd.fields.reserved2 = 0; 
    cmd.fields.reserved3 = 0;
    cmd.fields.coding = AD7490_CODING_BINARY; 
    cmd.fields.range = AD7490_RANGE_REF; 
    cmd.fields.weak_tri = AD7490_DOUT_TRI; 
    cmd.fields.shadow = AD7490_SHADOW_DISABLE; 
    cmd.fields.pm0 = (AD7490_PM_NORMAL >> 0) & 1; 
    cmd.fields.pm1 = (AD7490_PM_NORMAL >> 1) & 1; 
    cmd.fields.add0 = (channel >> 0) & 1; 
    cmd.fields.add1 = (channel >> 1) & 1; 
    cmd.fields.add2 = (channel >> 2) & 1;     
    cmd.fields.add3 = (channel >> 3) & 1; 
    cmd.fields.seq = AD7490_SEQ_DISABLE; 
    cmd.fields.write = AD7490_WRITE_ENABLE;

    return cmd.value;
}

void ad7490_set_multi_channel(ad7490_channel_t channel, unsigned int8 num_channels)
{
    ad7490_cmd_t cmd;
    // Set default values for all fields
    cmd.fields.reserved0 = 0; 
    cmd.fields.reserved1 = 0; 
    cmd.fields.reserved2 = 0; 
    cmd.fields.reserved3 = 0;
    cmd.fields.coding = AD7490_CODING_BINARY; 
    cmd.fields.range = AD7490_RANGE_REF; 
    cmd.fields.weak_tri = AD7490_DOUT_TRI; 
    cmd.fields.shadow = AD7490_SHADOW_DISABLE; 
    cmd.fields.pm0 = (AD7490_PM_NORMAL >> 0) & 1; 
    cmd.fields.pm1 = (AD7490_PM_NORMAL >> 1) & 1; 
    cmd.fields.add0 = (channel >> 0) & 1; 
    cmd.fields.add1 = (channel >> 1) & 1; 
    cmd.fields.add2 = (channel >> 2) & 1;     
    cmd.fields.add3 = (channel >> 3) & 1; 
    cmd.fields.seq = AD7490_SEQ_ENABLE; 
    cmd.fields.write = AD7490_WRITE_ENABLE;
}

void ad7490_init()
{
    fprintf(PC, "AD7490 Initialize\r\n");
    output_high(ADC_CS); 
    delay_ms(10); 
    unsigned int16 readdata = ad7490_read(ADC_CIGS1_CURR);
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
    delay_us(1); 
    spi_xfer(ADC_STREAM, cmd); 
    #ifdef AD7490_DEBUG
       fprintf(PC,"\t[ADC] <<< %04LX\r\n", cmd);
    #endif
    output_high(ADC_CS); 
    delay_us(1);

    // Read the ADC data
    output_low(ADC_CS); 
    delay_us(1);
    unsigned int16 ans = spi_xfer(ADC_STREAM, 0x0000);
    output_high(ADC_CS); 
    unsigned int16 readdata = ans & 0x0FFF; //Conver LSB <--> MSB
    
    #ifdef AD7490_DEBUG
        fprintf(PC,"\t[ADC] >>> %04LX\r\n", readdata);
    #endif
    return readdata;
}

void ad7490_read_2port(unsigned int8 channel1, unsigned int8 channel2, unsigned int16 *data1, unsigned int16 *data2)
{   
    unsigned int16 cmd1 = ad7490_make_cmd(channel1);
    unsigned int16 cmd2 = ad7490_make_cmd(channel2);
    // fprintf(PC, "\t[ADC] Command: %04LX\r\n", cmd);
    output_low(ADC_CS); 
    delay_us(1); 
    spi_xfer(ADC_STREAM, cmd1); 
    #ifdef AD7490_DEBUG
       fprintf(PC,"\t[ADC] <<< %04LX\r\n", cmd1);
    #endif
    output_high(ADC_CS); 
    delay_us(1);

    // Read the ADC data
    output_low(ADC_CS); 
    delay_us(1);
    unsigned int16 ans1 = spi_xfer(ADC_STREAM, cmd2);
    #ifdef AD7490_DEBUG
       fprintf(PC,"\t[ADC] <<< %04LX\r\n", cmd2);
    #endif
    output_high(ADC_CS); 
    delay_us(1);
    unsigned int16 ans2 = spi_xfer(ADC_STREAM, 0x0000);
    output_high(ADC_CS); 
    unsigned int16 readdata1 = ans1 & 0x0FFF; //Conver LSB <--> MSB
    unsigned int16 readdata2 = ans2 & 0x0FFF; //Conver LSB <--> MSB
    
    #ifdef AD7490_DEBUG
        fprintf(PC,"\t[ADC] >>> %04LX\r\n", readdata1);
        fprintf(PC,"\t[ADC] >>> %04LX\r\n", readdata2);
    #endif
    *data1 = readdata1;
    *data2 = readdata2;
}

void ad7490_setting_multi_channel(ad7490_channel_t channel, unsigned int8 num_channels)
{
    
    
    output_low(CS_PIN);
    



}


// End of file
