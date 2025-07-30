#ifndef MMJ_CIGS_FUNC_H
#define MMJ_CIGS_FUNC_H

/*
// _________ values _________
#define MISSION_DATA_SIZE 64
volatile unsigned int8 mission_datas[MISSION_DATA_SIZE] = {0x00};
volatile int8 executed_mission_count = 0;
*/

#define START_MAKER 0xFF
#define RESERVED_VALUE 0x00






// _________ functions ____________
#define DATA_BUFFER_SIZE 150
#define HEADER_SIZE 12
volatile unsigned int16 data_buffer [DATA_BUFFER_SIZE*2] = {0x00} ;
volatile unsigned int32 measured_time = 0 ;
volatile unsigned int16 measured_open_voltage = 0 ;
volatile unsigned int16 measured_pd = 0 ;
volatile unsigned int16 measured_temp_top = 0 ;
volatile unsigned int16 measured_temp_bot = 0 ;     


void make_meas_header(unsigned int8 *packetdata, unsigned int8 *cmd);


void sweep(unsigned int8 parameter[]);



typedef struct{
    unsigned int32 time;
    unsigned int16 temp_py_top;
    unsigned int16 temp_py_bot;
    unsigned int16 temp_mis7;
    unsigned int16 pd;
} MEASUREMENT_DATA;




#endif
//------------------End of File------------------
