#include "mcp4901.h"

unsigned int16 mcp4901_make_cmd(unsigned int16 value)
{
   mcp4901_cmd_t cmd;
   cmd.fields.startbit = 0; // Start bit
   cmd.fields.buffer = 1; // Buffer used
   cmd.fields.gain = 1; // Gain 1x
   cmd.fields.shutdown = 1; // Normal operation
   cmd.fields.data7 = (value >> 7) & 0x01;
   cmd.fields.data6 = (value >> 6) & 0x01;
   cmd.fields.data5 = (value >> 5) & 0x01;
   cmd.fields.data4 = (value >> 4) & 0x01;
   cmd.fields.data3 = (value >> 3) & 0x01;
   cmd.fields.data2 = (value >> 2) & 0x01;
   cmd.fields.data1 = (value >> 1) & 0x01;
   cmd.fields.data0 = value & 0x01; // LSB of the data
   cmd.fields.reserved = 0; // Reserved bits
   cmd.fields.reserved2 = 0; // Reserved bits
   cmd.fields.reserved3 = 0; // Reserved bits
   cmd.fields.reserved4 = 0; // Reserved bits

   return cmd.value;
}
