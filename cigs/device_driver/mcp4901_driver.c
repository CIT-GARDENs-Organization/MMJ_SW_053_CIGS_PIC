#include "mcp4901_driver.h"                        
#include "../hal/mmj_cigs_config.h"             
#include "../lib/device/mcp4901.h"       

// #define MCP4901_DEBUG

void mcp4901_init()
{
    fprintf(PC, "MCP4901 Initialize\r\n");
    output_high(DAC1_CS); 
    output_high(DAC2_CS); 
    delay_ms(10); 
    mcp4901_1_write(10); 
    mcp4901_2_write(10); 
    unsigned int16 readdata;
    readdata = ad7490_read(ADC_CIGS1_VREF); 
    fprintf(PC, "%04LX\r\n", readdata);
    if (readdata == 0xFFFF || readdata == 0x0000) {
        fprintf(PC, "\t[DAC Port 1] Connect error!\r\n");
    } else {
        fprintf(PC, "\t[DAC Port 1] Connect successful\r\n");
    }
    fprintf(PC, "%04LX\r\n", readdata);
    readdata = ad7490_read(ADC_CIGS2_VREF); 
    if (readdata == 0xFFFF || readdata == 0x0000) {
        fprintf(PC, "\t[DAC Port 2] Connect error!\r\n");
    } else {
        fprintf(PC, "\t[DAC Port 2] Connect successful\r\n");
    }
    fprintf(PC, "\tComplete\r\n");
}


void mcp4901_1_write(unsigned int16 value)
{
    unsigned int16 cmd = mcp4901_make_cmd(value);
    #ifdef MCP4901_DEBUG
      fprintf(PC, "\t[DAC] <<< %04LX\r\n", cmd);
    #endif
    output_low(DAC1_CS); 
    delay_us(10); 
    spi_xfer(DAC_STREAM, cmd); 
    output_high(DAC1_CS); 
}

void mcp4901_2_write(unsigned int16 value)
{
   unsigned int16 cmd = mcp4901_make_cmd(value);
   #ifdef MCP4901_DEBUG
      fprintf(PC, "\t[DAC] <<< %04LX\r\n", cmd);
   #endif
   output_low(DAC2_CS); 
   delay_us(100); 
   spi_xfer(DAC_STREAM, cmd); 
   output_high(DAC2_CS); 
}
// End of file
