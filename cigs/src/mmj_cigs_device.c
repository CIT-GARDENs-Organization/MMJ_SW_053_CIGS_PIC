#include "../mmj_cigs_device.h"
#include "../mmj_cigs_config.h"
#include "../../lib/device/mcp4901.h"
#include "../../lib/device/ad7490.h"


void ad7490_init()
{
    fprintf(PC, "AD7490 Initialize\r\n");
    output_high(ADC_CS); // Set CS pin high to deselect the ADC
    delay_ms(10); // Wait for the ADC to stabilize
    unsigned int16 readdata = ad7490_read(ADC_TEMP_PY_TOP); // Read a dummy value to initialize the ADC
    if (readdata == 0xFFFF) {
        fprintf(PC, "\t[ADC] Connect error!\r\n");
    } else {
        fprintf(PC, "\t[ADC] Connect successful\r\n");
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

void mcp4901_init()
{
    fprintf(PC, "MCP4901 Initialize\r\n");
    output_high(DAC1_CS); // Set CS pin high to deselect the DAC
    output_high(DAC2_CS); // Set CS pin high to deselect the second DAC
    delay_ms(10); // Wait for the DAC to stabilize
    mcp4901_1_write(0); // Initialize DAC 1 to 0
    mcp4901_2_write(0); // Initialize DAC 2 to 0
    unsigned int16 readdata;
    readdata = ad7490_read(ADC_CIGS1_DAC); // Read a dummy value to initialize the DAC
    if (readdata == 0xFFFF) {
        fprintf(PC, "\t[DAC Port 1] Connect error!\r\n");
    } else {
        fprintf(PC, "\t[DAC Port 1] Connect successful\r\n");
    }
    readdata = ad7490_read(ADC_CIGS2_DAC); // Read a dummy value to initialize the second DAC
    if (readdata == 0xFFFF) {
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
   output_low(DAC1_CS); // Select MCP4901
   delay_us(100); // Ensure CS is stable before sending data
   spi_xfer(ADCDAC_STREAM, cmd); // Send first byte
   output_high(DAC1_CS); // Deselect MCP4901
}

void mcp4901_2_write(unsigned int16 value)
{
   unsigned int16 cmd = mcp4901_make_cmd(value);
   #ifdef MCP4901_DEBUG
      fprintf(PC, "\t[DAC] <<< %04LX\r\n", cmd);
   #endif
   output_low(DAC2_CS); // Select MCP4901
   delay_us(100); // Ensure CS is stable before sending data
   spi_xfer(ADCDAC_STREAM, cmd); // Send first byte
   output_high(DAC2_CS); // Deselect MCP4901
}

