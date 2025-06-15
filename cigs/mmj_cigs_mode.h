#ifndef MMJ_CIGS_MODE_H
#define MMJ_CIGS_MODE_H


// ___________MODE FUNCTIONS____________

#Separate
void mode_measure(unsigned int8 parameter[]);

// Flash command 
#Separate
void mode_flash_erase_all();
#Separate
void mode_flash_erase_1sector();
#Separate
void mode_flash_copy_1sector();
#Separate
void mode_flash_write_1sector();
#Separate
void mode_flash_func2();
#Separate
void mode_flash_read();
#Separate
void mode_flash_read_address();

// IV command
#Separate
void mode_iv_measure();
#Separate
void mode_iv_measure_voltage();
#Separate
void mode_iv_meas_adc();void mode_iv_meas_adc();



// Development command
#Separate
void mode_dev_vol();
#Separate
void mode_dev_cur();
#Separate
void mode_dev_temp();   
#Separate
void mode_dev_pd();
#Separate
void mode_dev_cigs();
#Separate
void mode_dev_time();
#Separate
void mode_dev_sweep();

// ______________CMD ID________________
#define ID_SWEEP 0x01

// Flash command 
#define ID_FLASH_ERASE_ALL 0xA0
#define ID_FLASH_ERASE_1SECTOR 0xA1
#define ID_FLASH_COPY_1SECTOR 0xA2
#define ID_FLASH_WRITE_1SECTOR 0xA3
#define ID_FLASH_FUNC2 0xA4
#define ID_FLASH_READ 0xA5
#define ID_FLASH_READ_ADDRESS 0xA6

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

#include "src/mmj_cigs_mode.c"

#endif // MMJ_CIGS_MODE_H

//------------------End of File------------------
