// #include "ad7490.h"  // ヘッダーファイルから自動的にインクルードされるため不要

unsigned int16 ad7490_readdata(unsigned int16 channel)
{
    spi_xfer(ADC_STREAM, channel); // Dummy transfer to start communication
    #ifdef AD7490_DEBUG
        fprintf(PC, "\t[ADC] <<< %04LX\r\n", channel);
    #endif
    unsigned int16 ans = spi_xfer(ADC_STREAM);
    return ans & 0x0FFF; //Conver LSB <--> MSB
}

unsigned int16 ad7490_make_cmd(int8 channel)
{
    ad7490_cmd_t cmd;
    
    cmd.fields.reserved0 = 0; // Reserved bits
    cmd.fields.reserved1 = 0; // Reserved bits
    cmd.fields.reserved2 = 0; // Reserved bits
    cmd.fields.reserved3 = 0;
    cmd.fields.coding = 1; 
    cmd.fields.range = 0; 
    cmd.fields.weak_tri = 0; 
    cmd.fields.shadow = 0; 
    cmd.fields.pm0 = AD7490_PM_AUTO_STANDBY & 0b00000001; // Power mode bits
    cmd.fields.pm1 = (AD7490_PM_AUTO_STANDBY >> 1) & 0b00000001; // Power mode bits
    cmd.fields.add0 = (channel >> 0) & 0b00000001; // Channel address bits
    cmd.fields.add1 = (channel >> 1) & 0b00000001; // Channel address bits
    cmd.fields.add2 = (channel >> 2) & 0b00000001; // Channel address bits
    cmd.fields.add3 = (channel >> 3) & 0b00000001; // Channel address bits
    cmd.fields.seq = 0; // Random mode
    cmd.fields.write = 1; // Read operation


    return cmd.value;
}
