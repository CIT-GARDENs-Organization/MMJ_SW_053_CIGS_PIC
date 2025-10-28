#ifndef mmj_cigs_iv_H
#define mmj_cigs_iv_H

/*
// _________ values _________
#define MISSION_DATA_SIZE 64
volatile unsigned int8 mission_datas[MISSION_DATA_SIZE] = {0x00};
volatile int8 executed_mission_count = 0;
*/

#define START_MAKER 0xFF
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
    unsigned int8 data_buffer[0x200];
    int1 active;
} sweep_config_t;

typedef struct{
    unsigned int8 port_num; 
    unsigned int16 sweep_step;
    iv_data_t data_buffer[0xFF];
    int1 active;
} sweep_debug_config_t;

typedef struct 
{
    unsigned int8 data[3];
} meas_data_t;

#define PACKET_IV_HEADER_SIZE 51
#define IV_HEADER_SIZE 17
#define PACKET_IV_DATA_SIZE  63
#define IV_DATA_SIZE  21

typedef union{
    unsigned int8 raw[PACKET_SIZE];
    struct {
        unsigned int8 start_marker;
        unsigned int32 time_sec;
        unsigned int8 time_msec;
        meas_data_t env_data[2];
        unsigned int8 ivdata[PACKET_IV_HEADER_SIZE];
        unsigned int8 crc;
    }header;
    struct {
        unsigned int8 iv_data[PACKET_IV_DATA_SIZE];
        unsigned int8 crc;
    }data;
} iv_data_packet_t;



iv_env_t create_meas_data();
void log_meas_data(iv_env_t *environment_data_ptr, sweep_config_t *port_data);
void log_meas_data_with_print(iv_env_t *environment_data_ptr, sweep_config_t *port_data);

void sweep_with_print();

unsigned int16 calc_pd_value(unsigned int16 data);
// 
unsigned int16 calc_curr_value(unsigned int16 data);
#define SHUNT_RESISTANCE_OHM 0.05
#define AMP_GAIN 200
#define ADC_REF_VOLTAGE_MV 2500 
#define ADC_MAX_READING 4095
#define ADC_CURR_REF_VOLTAGE_MV 1250

#define SWEEP_MAX_STEP 0xFF

typedef enum {
    CELL1 = 0,
    CELL2,
} cell_select_t;

typedef struct {
    cell_select_t cell;
    unsigned int16 voltage[SWEEP_MAX_STEP];
    unsigned int16 current[SWEEP_MAX_STEP];
    unsigned int16 max_power;
    unsigned int16 sweep_count;
} cell_iv_data_t;

typedef struct {
    unsigned int32 time;
    unsigned int16 temp_mis7;
    unsigned int16 temp_py_top;
    unsigned int16 temp_py_bot;
    unsigned int16 pd;
} env_data_t;

typedef struct{
    cell_iv_data_t cell1_iv_data;
    cell_iv_data_t cell2_iv_data;
} sweep_result_t;

typedef struct {
    int1 cell1;
    int1 cell2;
    unsigned int16 curr_limit;
}sweep_setting_t;


void meas_env_data(env_data_t *env_data_ptr);
void meas_iv(sweep_setting_t *sweep_config_ptr, sweep_result_t *sweep_result_ptr);
void save_sweep_result(env_data_t *env_data_ptr, sweep_result_t *sweep_result_ptr);

#endif
//------------------End of File------------------
