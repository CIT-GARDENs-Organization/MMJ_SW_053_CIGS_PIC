#include "../ad7490.h"



//#define AD7490_DEBUG

unsigned int16 ad7490_readdata(unsigned int16 channel)
{
    //output_low(ADC_CS); // Set CS pin low to select the ADC
    spi_xfer(ADC_STREAM, channel); // Dummy transfer to start communication
    //output_high(ADC_CS); // Set CS pin high to end communication
    #ifdef AD7490_DEBUG
        fprintf(PC, "\t[ADC] << %04LX\r\n", channel);
    #endif
    //output_high(ADC_CS); // Set CS pin high to end communication
    //delay_us(10); // Wait for the ADC to process the command
    //output_low(ADC_CS); // Set CS pin low to select the ADC again
    unsigned int16 ans = spi_xfer(ADC_STREAM);
    //output_high(ADC_CS); // Set CS pin low to select the ADC again
    return ans & 0x0FFF; //Conver LSB <--> MSB
}



void ad7490_init()
{  
    fprintf(PC, "AD7490 Initialize\r\n");
    // Set default register values
    reg.fields.coding = 1; // 1: Two's complement, 0: Binary
    reg.fields.write = 1;
    reg.fields.seq = 0; 
    reg.fields.range = 1;
    reg.fields.weak_tri = 0; // 1: Weak trigger, 0: Strong trigger
    reg.fields.shadow = 0; // 1: Shadow register, 0: Direct access
    coding = 1;
    write = 1;
    seq = 0;
    range = 1;
    weak_tri = 0;
    shadow = 0;
    fprintf(PC, "\tComplete\r\n");
}

unsigned int16 ad7490_read(int8 channel)
{   
    //AD7490_RESISTOR reg;
        reg.fields.coding = 1; // 1: Two's complement, 0: Binary
    reg.fields.write = 1;
    reg.fields.seq = 0; 
    reg.fields.range = 1;
    reg.fields.weak_tri = 0; // 1: Weak trigger, 0: Strong trigger
    reg.fields.shadow = 0; // 1: Shadow register, 0: Direct access
    set_add(channel);
    set_pm(AD7490_PM_AUTO_STANDBY); // Set power mode to normal

    unsigned int16 cmd = ad7490_make_cmd(channel);
    
    output_low(ADC_CS); // Set CS pin low to select the ADC
    spi_xfer(ADC_STREAM, cmd); // Dummy transfer to start communication
    #ifdef AD7490_DEBUG
        //fprintf(PC, "AD7490 Read Channel: %d\r\n", channel);
        fprintf(PC,"\t[ADC] << %04LX\r\n", reg.value);
    #endif
    output_high(ADC_CS); // Set CS pin high to end communication
    delay_us(10); // Wait for the ADC to process the command
    output_low(ADC_CS); // Set CS pin low to select the ADC again
    unsigned int16 ans = spi_xfer(ADC_STREAM);
    output_high(ADC_CS); // Set CS pin high to end communication
    unsigned int16 readdata = ans & 0x0FFF; //Conver LSB <--> MSB
    //unsigned int16 readdata = 0;
    
    #ifdef AD7490_DEBUG
        fprintf(PC,"\t[ADC] >> %04LX\r\n", readdata);
    #endif
    return readdata;
}

void set_add(int8 channel)
{
    reg.fields.add0 = (channel >> 0) & 0x1;
    reg.fields.add1 = (channel >> 1) & 0x1;
    reg.fields.add2 = (channel >> 2) & 0x1;
    reg.fields.add3 = (channel >> 3) & 0x1;
}

void set_pm(int8 powermode)
{
    reg.fields.pm0 = (powermode >> 0) & 0x1;
    reg.fields.pm1 = (powermode >> 1) & 0x1;
    
}

void set_blank()
{
    reg.fields.blank0 = 0;
    reg.fields.blank1 = 0;
    reg.fields.blank2 = 0;
    reg.fields.blank3 = 0;
}

unsigned int16 ad7490_make_cmd(int8 channel)
{
    unsigned int16 cmd = 0x0000;

    // WRITE (Bit11)
    cmd |= (write & 0x01) << 11;
    fprintf(PC, "\tWRITE : %d (%04LX)\r\n", write, cmd);

    // SEQ (Bit10)
    cmd |= (seq & 0x01) << 10;
    fprintf(PC, "\tSEQ   : %d (%04LX)\r\n", seq, cmd);

    // ADDR (Bit9-6)
    cmd |= (channel & 0x0F) << 6;
    fprintf(PC, "\tADDR  : %d (%04LX)\r\n", channel, cmd);

    // PM1/PM0 (Bit5-4)
    cmd |= (AD7490_PM_AUTO_STANDBY & 0x03) << 4;
    fprintf(PC, "\tPM     : %d (%04LX)\r\n", AD7490_PM_AUTO_STANDBY, cmd);

    // SHADOW (Bit3)
    cmd |= (shadow & 0x01) << 3;
    fprintf(PC, "\tSHADOW : %d (%04LX)\r\n", shadow, cmd);

    // WEAK/TRI (Bit2)
    cmd |= (weak_tri & 0x01) << 2;
    fprintf(PC, "\tWEAK/TRI : %d (%04LX)\r\n", weak_tri, cmd);

    // RANGE (Bit1)
    cmd |= (range & 0x01) << 1;
    fprintf(PC, "\tRANGE  : %d (%04LX)\r\n", range, cmd);

    // CODING (Bit0)
    cmd |= (coding & 0x01) << 0;
    fprintf(PC, "\tCODING : %d (%04LX)\r\n", coding, cmd);

    // コマンドを左詰め (12bit→16bit)
    cmd <<= 4;
    fprintf(PC, "\t[AD7490] Final Command : %04LX\r\n", cmd);

    return cmd;
}
