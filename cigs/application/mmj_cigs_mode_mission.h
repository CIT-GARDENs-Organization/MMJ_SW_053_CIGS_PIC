#ifndef MMJ_CIGS_MODE_MISSION_H
#define MMJ_CIGS_MODE_MISSION_H

#define PARAMETER_LENGTH 9

// ___________MODE FUNCTIONS____________


void mode_dummy(unsigned int8 uplinkcmd[]);
typedef struct {
    unsigned int8 id;
    unsigned int32 param1;
    unsigned int16 param2;
} DUMMY_CMD;

// IV command

void mode_iv_test(unsigned int8 *uplinkcmd);

void mode_test_iv(unsigned int8 *uplinkcmd);


void mode_meas_enviro();
// ______________CMD ID________________

// IV command
#define ID_IV_MEASURE 0xB0
#define ID_IV_MEASURE_VOLTAGE 0xB1

// Development command
#define ID_MEAS_IV 0xA0
#define ID_MEAS_IV_CURR_SELECT 0xA1
#define ID_MEAS_IV_PD_SELECT 0xA2
#define ID_MEAS_IV_CURR_AND_PD_SELECT 0xA3
#define ID_MEAS_ENVIRONMENT 0xA4
#define ID_MEAS_ENVIRONMENT_DEBUG 0xA5
#define ID_MEAS_DEBUG 0xA6

void mode_meas_iv(unsigned int8 *uplinkcmd);


typedef union {
    unsigned int8 raw;
    struct {
        unsigned int8 mission_continue : 1;
        unsigned int8 erase_and_save : 1;
        unsigned int8 reserved : 6;
    } flag;
} mission_state_t;


typedef struct {
    unsigned int8 id; // Command ID
    unsigned int16 sleep_time; // Sleep time in milliseconds
    unsigned int16 curr_threshold; // Current threshold for measurement
    unsigned int16 pd_threshold; // PD threshold for measurement
    unsigned int16 curr_limit; // Current limit for measurement
    unsigned int16 meas_time; // Measurement time in milliseconds
    mission_state_t mission_state; // Flag to indicate if the measurement is finished
} MEAS_IV_CMD;

MEAS_IV_CMD make_meas_iv_cmd(unsigned int8 uplinkcmd[]);




#define ID_DEV_CUR 0xC1
#define ID_DEV_TEMP 0xC1
#define ID_DEV_PD 0xC2
#define ID_DEV_CIGS 0xC3
#define ID_DEV_TIME 0xC4
#define ID_DEV_SWEEP 0xC5








#endif // MMJ_CIGS_MODE_MISSION_H

//------------------End of File------------------
