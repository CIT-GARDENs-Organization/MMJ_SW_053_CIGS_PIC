#ifndef MMJ_CIGS_MODE_FLASH_H
#define MMJ_CIGS_MODE_FLASH_H

#ifndef UPLINK_COMMAND_SIZE
    #define UPLINK_COMMAND_SIZE 9
#endif


// ___________FLASH COMMAND STRUCT____________
typedef union {
    unsigned int8 raw[UPLINK_COMMAND_SIZE];
    struct {
        unsigned int8 command_id;
        unsigned int32 address;
        unsigned int8 blank1;
        unsigned int16 packet_num;
        unsigned int8 sector_num;
    } param_t;
} flash_command_t;




// ___________MISF FUNCTIONS____________
// Command ID 
#define ID_MISF_ERASE_ALL 0x80
#define ID_MISF_ERASE_1SECTOR 0x81
#define ID_MISF_ERASE_4kByte_SUBSECTOR 0x82
#define ID_MISF_ERASE_64kByte_SUBSECTOR 0x83
#define ID_MISF_WRITE_DEMO 0x84
#define ID_MISF_WRITE_4kByte_SUBSECTOR 0x85
#define ID_MISF_READ 0x86
#define ID_MISF_READ_ADDRESS 0x87
#define ID_MISF_ERASE_AND_RESET 0x88
#define ID_MISF_ADDRESS_RESET 0x8F

// Mode
void mode_misf_erase_all(unsigned int8 *uplink_cmd_ptr);
void mode_misf_erase_1sector(unsigned int8 *uplink_cmd_ptr);
void mode_misf_erase_4kbyte_subsector(unsigned int8 *uplink_cmd_ptr);
void mode_misf_erase_64kbyte_subsector(unsigned int8 *uplink_cmd_ptr);
void mode_misf_write_demo(unsigned int8 *uplink_cmd_ptr);
void mode_misf_write_4kbyte_subsector(unsigned int8 *uplink_cmd_ptr);
void mode_misf_read(unsigned int8 *uplink_cmd_ptr);
void mode_misf_read_address(unsigned int8 *uplink_cmd_ptr);
void mode_misf_erase_and_reset(unsigned int8 *uplink_cmd_ptr);
void mode_misf_address_reset(unsigned int8 *uplink_cmd_ptr);


// ___________SMF FUNCTIONS____________
// Command
#define ID_SMF_COPY 0x90
#define ID_SMF_READ 0x91
#define ID_SMF_ERASE 0x92
#define ID_SMF_COPY_FORCE 0x93
#define ID_SMF_READ_FORCE 0x94
#define ID_SMF_ERASE_FORCE 0x95
#define ID_SMF_RESET 0x96

// Mode
void mode_smf_copy(unsigned int8 *uplink_cmd_ptr);
void mode_smf_read(unsigned int8 *uplink_cmd_ptr);
void mode_smf_erase(unsigned int8 *uplink_cmd_ptr);
void mode_smf_copy_force(unsigned int8 *uplink_cmd_ptr);
void mode_smf_read_force(unsigned int8 *uplink_cmd_ptr);
void mode_smf_erase_force(unsigned int8 *uplink_cmd_ptr);
void mode_smf_address_reset(unsigned int8 *uplink_cmd_ptr);


#endif // MMJ_CIGS_MODE_FLASH_H

//------------------End of File------------------
