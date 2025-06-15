#include "../ad7490.h"


union AD7490_RESISTOR reg;
struct AD7490_STREAM adf7490_stream;


void ad7490_init()
{  
    // Set default register values
    reg.coding = 0; // 1: Two's complement, 0: Binary
    reg.write = 1;
    reg.seq = 0; 
    reg.randge = 0;
    reg.weak_tri = 0; // 1: Weak trigger, 0: Strong trigger
    reg.shadow = 0; // 1: Shadow register, 0: Direct access

}

unsigned int16 ad7490_read(int16 channel)
{   
    set_add(channel);
    set_pm(AD7490_PM_NORMAL);
    //spi_xfer(adf7490_stream.spi_stream, reg.value); // Dummy transfer to start communication
    #ifdef AD7490_DEBUG
        fprintf(PC,"\t[ADC] << %04X\r\n", reg.value);
    #endif
    
    //unsigned int16 ans = spi_xfer(adf7490_stream.spi_stream);
    //unsigned int16 readdata = ans & 0x0FFF; //Conver LSB <--> MSB
    unsigned int16 readdata = 0;
    
    #ifdef AD7490_DEBUG
        //fprintf(PC,"\t[ADC] >> %04X\r\n", readdata);
    #endif
    return readdata;
}

void set_add(int16 channel)
{
    reg.add0 = (channel >> 0) & 0x1;
    reg.add1 = (channel >> 1) & 0x1;
    reg.add2 = (channel >> 2) & 0x1;
    reg.add3 = (channel >> 3) & 0x1;
}

void set_pm(int8 powermode)
{
    reg.pm0 = (powermode >> 0) & 0x1;
    reg.pm1 = (powermode >> 1) & 0x1;
    
}
