#ifndef MMJ_CIGS_DEVICE_H
#define MMJ_CIGS_DEVICE_H

// Instances of device drivers
// AD7490: ADC
// MCP4901: DAC


// AD7490 Functions
void ad7490_init(void);
unsigned int16 ad7490_read(int8 channel);

// MCP4901 Functions
void mcp4901_init();
void mcp4901_1_write(unsigned int16 val);
void mcp4901_2_write(unsigned int16 val);

#endif // MMJ_CIGS_DEVCE_H
//------------------End of File------------------
