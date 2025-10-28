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
    cmd.fields.coding = 1; // Two's complement
    cmd.fields.range = 0; // Normal mode
    cmd.fields.weak_tri = 0; // Strong trigger
    cmd.fields.shadow = 0; // Direct access
    cmd.fields.pm0 = AD7490_PM_NORMAL & 0b00000001; // Power mode bits
    cmd.fields.pm1 = (AD7490_PM_NORMAL >> 1) & 0b00000001; // Power mode bits
    cmd.fields.add0 = (channel >> 0) & 0b00000001; // Channel address bits
    cmd.fields.add1 = (channel >> 1) & 0b00000001; // Channel address bits
    cmd.fields.add2 = (channel >> 2) & 0b00000001; // Channel address bits
    cmd.fields.add3 = (channel >> 3) & 0b00000001; // Channel address bits
    cmd.fields.seq = 0; // Random mode
    cmd.fields.write = 1; // Read operation
    return cmd.value;
}

unsigned int16 ad7490_cmd(ad7490_config_t *config)
{
    ad7490_cmd_t cmd;
    
    cmd.fields.reserved0 = 0; 
    cmd.fields.reserved1 = 0; 
    cmd.fields.reserved2 = 0; 
    cmd.fields.reserved3 = 0;
    cmd.fields.coding = config->coding; 
    cmd.fields.range = config->range; 
    cmd.fields.weak_tri = config->dout_mode; 
    cmd.fields.shadow = config->shadow; 
    cmd.fields.pm0 = (config->power_mode >> 0) & 1; 
    cmd.fields.pm1 = (config->power_mode >> 1) & 1; 
    // Channel bits should be set separately after this function
    cmd.fields.seq = config->seq_mode; 
    cmd.fields.write = AD7490_WRITE_ENABLE;
    return cmd.value;
}

static void ad7490_enable_cs(void)
{
    output_low(ADC_CS); 
}

static void ad7490_disable_cs(void)
{
    output_high(ADC_CS); 
}