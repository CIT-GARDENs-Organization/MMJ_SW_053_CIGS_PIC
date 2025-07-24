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

void mode_iv_measure();

void mode_iv_measure_voltage();

void mode_iv_meas_adc();

void mode_test_iv(unsigned int8 *uplinkcmd[]);

// Development command

void mode_dev_vol();

void mode_dev_cur();

void mode_dev_temp();   

void mode_dev_pd();

void mode_dev_cigs();

void mode_dev_time();

void mode_dev_sweep();


void mode_sweep_port1(unsigned int8 uplinkcmd);

// ______________CMD ID________________

// IV command
#define ID_IV_MEASURE 0xB0
#define ID_IV_MEASURE_VOLTAGE 0xB1

// Development command
#define ID_DEV_VOL 0xC0
#define ID_DEV_CUR 0xC1
#define ID_DEV_TEMP 0xC1
#define ID_DEV_PD 0xC2
#define ID_DEV_CIGS 0xC3
#define ID_DEV_TIME 0xC4
#define ID_DEV_SWEEP 0xC5








#endif // MMJ_CIGS_MODE_MISSION_H

//------------------End of File------------------
