#ifndef MMJ_CIGS_PIC_FUNC_H
#define MMJ_CIGS_PIC_FUNC_H

/*
// _________ values _________
#define MISSION_DATA_SIZE 64
volatile unsigned int8 mission_datas[MISSION_DATA_SIZE] = {0x00};
volatile int8 executed_mission_count = 0;
*/

/*

// Packet format
#define PACKET_SIZE 64
#define CHECKSUM
//volatile unsigned int8 PACKET_DATA[MISSION_DATA_SIZE] = {0x00};
*/



// _________ functions ____________
#define DATA_BUFFER_SIZE 150
#define HEADER_SIZE 12
volatile unsigned int16 data_buffer[DATA_BUFFER_SIZE*2] = {0x00};
volatile unsigned int8 cigs_data[DATA_BUFFER_SIZE*2] = {0x00};
volatile unsigned int8 cigs_data_header[HEADER_SIZE] = {0x00};
volatile unsigned int32 measured_time = 0;
volatile unsigned int16 measured_pd_start = 0;
volatile unsigned int16 measured_pd_end = 0;
volatile unsigned int16 measured_temp_top = 0;
volatile unsigned int16 measured_temp_bot = 0;

volatile unsigned int8 packet_data[64] = {0x00};

#separate
void sweep(unsigned int8 parameter[]);

void convert_cigs_data(unsigned int8 measurement_step);
void convert_header_data(void); 


#include "../mmj_cigs_pic_func.c"

#endif
//------------------End of File------------------