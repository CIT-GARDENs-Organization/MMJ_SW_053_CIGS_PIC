#ifndef MMJ_CIGS_MODE_MISSION_H
#define MMJ_CIGS_MODE_MISSION_H

#define CMD_LENGTH 9

// ______________CMD ID________________
// Operation Command
#define ID_MEAS_IV 0xA0
#define ID_MEAS_DEBUG 0xA1
#define ID_MEAS_ENV 0xA2
#define ID_MEAS_ENV_DEBUG 0xA3

// ______________MODE FUNCTIONS____________
void mode_meas_iv(unsigned int8 *uplinkcmd_ptr);
void mode_meas_iv_debug(unsigned int8 *uplinkcmd_ptr);
void mode_meas_env(unsigned int8 *uplinkcmd_ptr);
void mode_meas_env_debug(unsigned int8 *uplinkcmd_ptr);


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


#endif // MMJ_CIGS_MODE_MISSION_H

//------------------End of File------------------
