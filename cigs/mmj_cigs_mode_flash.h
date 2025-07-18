#ifndef MMJ_CIGS_MODE_FLASH_H
#define MMJ_CIGS_MODE_FLASH_H

#define PARAMETER_LENGTH 9

// ___________MODE FUNCTIONS____________

// Flash command 
#Separate
void mode_flash_erase_all(unsigned int8 parameter[]);
#Separate
void mode_flash_erase_1sector(unsigned int8 parameter[]);
#Separate
void mode_flash_erase_4kbyte_subsector(unsigned int8 parameter[]);

#separate
void mode_flash_write_demo(unsigned int8 parameter[]);
typedef struct {
    unsigned int8 id;
    unsigned int32 writeaddress;
    unsigned int16 packetnum;
} FLASH_WRITE_PARAM;

#Separate
void mode_flash_write_4kbyte_subsecotr(unsigned int8 parameter[]);

#Separate
void mode_flash_read(unsigned int8 parameter[]);
typedef struct{
    unsigned int8 id;
    unsigned int16 readpacketnum;
    unsigned int32 readaddress;
}FLASH_PARAM;

#Separate
void mode_flash_read_address(unsigned int8 parameter[]);

#Separate
void mode_flash_smf_copy(unsigned int8 parameter[]);

#Separate
void mode_flash_address_reset(unsigned int8 parameter[]);

#separate
void mode_flash_smf_read(unsigned int8 parameter[]);

// ______________CMD ID________________

// Flash command 
#define ID_FLASH_ERASE_ALL 0x90
#define ID_FLASH_ERASE_1SECTOR 0x91
#define ID_FLASH_ERASE_4kByte_SUBSECTOR 0x92
#define ID_FLASH_ERASE_64kByte_SUBSECTOR 0x93
#define ID_FALSH_WRITE_DEMO 0x94
#define ID_FLASH_WRITE_4kByte_SUBSECTOR 0x95
#define ID_FLASH_READ 0x96
#define ID_FLASH_READ_ADDRESS 0x97
#define ID_FLASH_SMF_COPY 0x9A
#define ID_FLASH_SMF_READ 0x9B
#define ID_FLASH_SMF_ERASE 0x9C
#define ID_FLASH_ADDRESS_RESET 0x9F


#include "src/mmj_cigs_mode_flash.c"

#endif // MMJ_CIGS_MODE_FLASH_H

//------------------End of File------------------
