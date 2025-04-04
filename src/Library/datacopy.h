#ifndef DATA_COPY_H
#define DATA_COPY_H

#define EXAMPLE_APRS_HEADER   0x00000000
#define EXAMPLE_APRS          0x00001000
#define EXAMPLE_SENSOR_HEADR  0x0000A000
#define EXAMPLE_SENSOR        0x00010000

void copy_data(void);

SmfDataStruct *dequeue_smf_data(void);

int1 is_empty_smf_data(void);

// SMF data queue
typedef struct{
    unsigned int32 src;
    unsigned int32 dest;
    unsigned int32 size;
 }SmfDataStruct;
 
 #define SMF_DATA_SIZE 16
 
 volatile int8 smf_data_head = 0;
 volatile int8 smf_data_tail = 0;
 
 SmfDataStruct smf_data[SMF_DATA_SIZE] = { {0x00} };

#endif
