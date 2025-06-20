#ifndef MMJ_CIGS_MODE_H
#define MMJ_CIGS_MODE_H

#define PARAMETER_LENGTH 9

// ___________MODE FUNCTIONS____________

#Separate
void mode_measure(unsigned int8 parameter[]);

// Flash command 
#Separate
void mode_flash_erase_all(unsigned int8 parameter[]);
#Separate
void mode_flash_erase_1sector(unsigned int8 parameter[]);
#Separate
void mode_flash_erase_4kbyte_subsector(unsigned int8 parameter[]);
#Separate
void mode_flash_write_demo(unsigned int8 parameter[]);
#Separate
void mode_flash_write_4kbyte_subsecotr(unsigned int8 parameter[]);
#Separate
void mode_flash_read(unsigned int8 parameter[]);
#Separate
void mode_flash_read_address(unsigned int8 parameter[]);
#Separate
void mode_flash_smf_copy(unsigned int8 parameter[]);
#Separate
void mode_flash_address_reset(unsigned int8 parameter[]);


// IV command
#Separate
void mode_iv_measure();
#Separate
void mode_iv_measure_voltage();
#Separate
void mode_iv_meas_adc();



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
#define ID_FLASH_ERASE_ALL 0x90
#define ID_FLASH_ERASE_1SECTOR 0x91
#define ID_FLASH_ERASE_4kByte_SUBSECTOR 0x92
#define ID_FALSH_WRITE_DEMO 0x93
#define ID_FLASH_4kByte_SUBSECTOR 0x94
#define ID_FLASH_READ 0x96
#define ID_FLASH_READ_ADDRESS 0x97
#define ID_FLASH_SMF_COPY 0x9A
#define ID_FLASH_ADDRESS_RESET 0x9F
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


typedef struct{
    unsigned int8 id;
    unsigned int16 readpacketnum;
    unsigned int32 readaddress;
}FLASH_PARAM;



#include "src/mmj_cigs_mode.c"

#endif // MMJ_CIGS_MODE_H

//------------------End of File------------------
