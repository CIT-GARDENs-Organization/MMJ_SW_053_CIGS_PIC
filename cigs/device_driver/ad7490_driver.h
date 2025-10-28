#ifndef AD7490_DRIVER_H
#define AD7490_DRIVER_H


// AD7490 Functions
void ad7490_init(void);
unsigned int16 ad7490_read(int8 channel);

static void ad7490_enable_cs(void);
static void ad7490_disable_cs(void);

#endif // MMJ_CIGS_DEVICE_H
//------------------End of File------------------
