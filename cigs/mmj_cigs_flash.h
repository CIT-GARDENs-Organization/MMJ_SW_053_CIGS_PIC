#ifndef  MMJ_CIGS_FLASH_H
#define  MMJ_CIGS_FLASH_H




// MIS_FM Function
void misf_init(void);
void update_misf_data_header(void);
void update_address_area(void);



#define PACKET_SIZE 64
#define MISF_DATA_HEADER_SIZE 64 // MISFデータヘッダーのサイズ
#define MISF_PICLOG_MAX_COUNT 60




// __________MISF-ADDRESS__________
#define ADDRESS_SMF_START 0x000000
#define ADDRESS_SMF_END   0xF42400
#define ADDRESS_MANEGE_START 0x000000
#define ADDRESS_MANEGE_END   0x000FFF
#define ADDRESS_MISF_PICLOG_INDEX_START 0x000100
#define ADDRESS_MISF_PICLOG_INDEX_END 0x0001FF
#define ADDRESS_MISF_PICLOG_DATA_START 0x000200
#define ADDRESS_MISF_PICLOG_DATA_END   0x141FFF
#define ADDRESS_MISF_MEASUREMENT_START 0x142000
#define ADDRESS_MISF_MEASUREMENT_END   0x941FFF

//
#define SECTOR_64K_BYTE 0x10000 // 64KByte
#define SECTOR_32K_BYTE 0x8000  // 32KByte
#define SECTOR_4K_BYTE  0x1000  // 4KByte

// Counter
unsigned int32  smf_piclog_use_counter;
unsigned int8   smf_piclog_loop_counter;
unsigned int32  smf_meas_use_counter;
unsigned int8   smf_meas_loop_counter;

unsigned int32  misf_piclog_use_counter;
unsigned int8   misf_piclog_loop_counter;
unsigned int32  misf_piclog_uncopyed_counter;
unsigned int8   misf_piclog_write_counter;
unsigned int32  misf_meas_use_counter;
unsigned int8   misf_meas_loop_counter;
unsigned int32  misf_meas_uncopyed_counter;


#define DATA_HEADER_SIZE 64 


typedef struct FLASH_DATA_HEADER {
    unsigned int32  smf_piclog_use_counter;
    unsigned int8   smf_piclog_loop_counter;
    unsigned int32  smf_meas_use_counter;
    unsigned int8   smf_meas_loop_counter;

    unsigned int32  misf_piclog_use_counter;
    unsigned int8   misf_piclog_loop_counter;
    unsigned int32  misf_piclog_uncopyed_counter;
    unsigned int8   misf_piclog_write_counter;
    unsigned int32  misf_meas_use_counter;
    unsigned int8   misf_meas_loop_counter;
    unsigned int32  misf_meas_uncopyed_counter;

    unsigned int8   reserve[34];
    unsigned int8   crc;
}   FLASH_DATA_HEADER; // フィールド




// Packet format
//unsigned int8 HEADER_SIZE 0x12

typedef union{
    struct {
        unsigned int8  header_start; // Header start byte
        unsigned int8  header_end;   // Header end byte
        unsigned int32 measurement_time; // Measurement time in milliseconds
        unsigned int16 measured_pd_start; // Start PD value
        unsigned int16 measured_pd_end;   // End PD value
        unsigned int16 measured_temp_top;  // Top temperature value
        unsigned int16 measured_temp_bot;  // Bottom temperature value
        unsigned int8  mode_data;          // Mode data
        unsigned int8  crc;                // CRC for error checking
    } fields;
    unsigned int8 bytes[PACKET_SIZE];
}MEAS_PACKET;







// ___________SMF-ADDRESS___________







#include "src/mmj_cigs_flash.c"

#endif // MMJ_CIGS_FLASH_H
//------------------End of File------------------
