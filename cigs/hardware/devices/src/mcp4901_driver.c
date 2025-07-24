#include "../mcp4901_driver.h"                        // 同じデバイスフォルダのヘッダー
#include "../../../system/mmj_cigs_config.h"             // システム設定
#include "../../../../lib/device/mcp4901.h"              // DACデバイスライブラリ

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
    fprintf(PC, "%04LX\r\n", readdata);
    if (readdata == 0xFFFF) {
        fprintf(PC, "\t[DAC Port 1] Connect error!\r\n");
    } else {
        fprintf(PC, "\t[DAC Port 1] Connect successful\r\n");
    }
    fprintf(PC, "%04LX\r\n", readdata);
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

