#ifndef CALC_TOOLS_H
#define CALC_TOOLS_H

int8 calc_crc8(unsigned int8 frame[], int8 payload_size);

void int32_to_be_array(int32 value, int8 buf[4]);

int32 be_array_to_int32(int8 buf[4]);

#include "calc_tools.c"
#endif // CALC_TOOLS_H