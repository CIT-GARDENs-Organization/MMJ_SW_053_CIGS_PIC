#ifndef mmj_cigs_iv_H
#define mmj_cigs_iv_H

/*
// _________ values _________
#define MISSION_DATA_SIZE 64
volatile unsigned int8 mission_datas[MISSION_DATA_SIZE] = {0x00};
volatile int8 executed_mission_count = 0;
*/

#define START_MAKER 0xFFFF
#define RESERVED_VALUE 0x00

#define HEADER_SIZE 5  // START_MAKER + time (4バイト)
#define ENV_SIZE    6  // pd + temp_py_top + temp_py_bot + temp_mis7 packed
#define DATA_SIZE_PER_STEP 3 // data0 + data1 12bitずつ


void make_meas_header(unsigned int8 *packetdata, unsigned int8 *cmd);


// void sweep(unsigned int8 parameter[]);

void sweep(unsigned int16 curr_threshold, unsigned int16 curr_limit, unsigned int16 pd_threshold);
void test_sweep(unsigned int16 curr_threshold, unsigned int16 curr_limit);

typedef struct{
    unsigned int32 time;
    unsigned int16 pd;
    unsigned int16 temp_py_top;
    unsigned int16 temp_py_bot;
    unsigned int16 temp_mis7;
} iv_env_t;

/*
typedef struct{
    unsigned int8 port_num; // Port number (1 or 2)
    unsigned int16 sweep_step; // Number of steps in the sweep
    unsigned int16 data_buffer[2][0xFF]; // Data buffer for voltage and current readings
    int1 active; // Indicates if the port is active (1 for active, 0 for inactive)
} sweep_config_t;
*/


typedef union{
    struct{
        unsigned int8 start_marker;
        unsigned int8 reserved;
        unsigned int8 command;
        unsigned int8 port_num;
        unsigned int16 data[2];
    }fields;

    unsigned int8 raw[PACKET_SIZE];
}iv_packet_t;

typedef struct {
    unsigned int16 voltage;
    unsigned int16 current;
} iv_data_t;

typedef struct{
    unsigned int8 port_num; 
    unsigned int16 sweep_step;
    iv_data_t data_buffer[0xFF];
    int1 active;
} sweep_config_t;

typedef struct 
{
    unsigned int8 data[3];
} meas_data_t;

typedef union{
    unsigned int8 raw[PACKET_SIZE-1];

    struct {
        unsigned int16 start_marker;
        unsigned int32 time_sec;
        unsigned int16 time_msec;
        meas_data_t env_data[2];
        meas_data_t iv_data[16];

    }header;
    struct {
        meas_data_t iv_data[21];
    }data;
} iv_data_packet_t;



iv_env_t create_meas_data();
void log_meas_data(iv_env_t *environment_data_ptr, sweep_config_t *port_data);
void log_meas_data_with_print(iv_env_t *environment_data_ptr, sweep_config_t *port_data);

void sweep_with_print();
#endif
//------------------End of File------------------
