#include "ad7490.h"

void ad7490_init()
{  
    fprintf(PC, "AD7490 Initialize\r\n");
    // Set default register values

    fprintf(PC, "\tComplete\r\n");
}

unsigned int16 ad7490_make_cmd(int8 channel)
{
    ad7490_cmd_t cmd;
    
    cmd.fields.reserved0 = 0; // Reserved bits
    cmd.fields.reserved1 = 0; // Reserved bits
    cmd.fields.reserved2 = 0; // Reserved bits
    cmd.fields.reserved3 = 0;
    cmd.fields.coding = 1; // Two's complement
    cmd.fields.range = 1; // Normal mode
    cmd.fields.weak_tri = 0; // Strong trigger
    cmd.fields.shadow = 0; // Direct access
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
