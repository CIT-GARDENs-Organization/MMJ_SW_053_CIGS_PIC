// ===================== mcp4911.h =====================
#ifndef _MCP4901_H_
#define _MCP4901_H_

typedef union {
   unsigned int16 value; // 16-bit value to write to the DAC
   struct {
      int1 reserved4; // Reserved bit, should be 0
      int1 reserved3; // Reserved bit, should be 0
      int1 reserved2; // Reserved bit, should be 0
      int1 reserved; // Reserved bit, should be 0
      int1 data0; // LSB of the data
      int1 data1; // 8-bit data to write to the DAC
      int1 data2; // 8-bit data to write to the DAC
      int1 data3; // 8-bit data to write to the DAC
      int1 data4; // 8-bit data to write to the DAC
      int1 data5; // 8-bit data to write to the DAC
      int1 data6; // 8-bit data to write to the DAC
      int1 data7; // 8-bit data to write to the DAC
      int1 shutdown; // 1: Shutdown mode, 0: Normal operation
      int1 gain; // 1: Gain 2x, 0: Gain 1x
      int1 buffer; // Buffer not used, should be 0
      int1 startbit; // Start bit, should be 1
   } fields;
} mcp4901_cmd_t;


// ---------------------- Public API --------------------------
void mcp4911_init();

unsigned int16 mcp4901_make_cmd(unsigned int16 value);

#include "mcp4901.c"

#endif /* _MCP4911_H_ */
// ===================== mcp4911.h =====================
