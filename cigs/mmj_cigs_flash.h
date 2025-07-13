#ifndef  MMJ_CIGS_FLASH_H
#define  MMJ_CIGS_FLASH_H




// MIS_FM Function
void misf_init(void);
void update_misf_data_header(void);
void write_misf_address_area(void);



#define PACKET_SIZE 64
#define MISF_DATA_HEADER_SIZE 64 // MISFデータヘッダーのサイズ
#define MISF_PICLOG_MAX_COUNT 60




// __________MISF-ADDRESS__________
#define ADDRESS_MISF_START              0x00000000
#define ADDRESS_MISF_END                0x00F42400
#define ADDRESS_MANEGE_START            0x00000000
#define ADDRESS_MANEGE_END              0x0000FFFF
#define ADDRESS_MISF_PICLOG_INDEX_START 0x00010000
#define ADDRESS_MISF_PICLOG_INDEX_END   0x0001FFFF
#define ADDRESS_MISF_PICLOG_DATA_START  0x00020000
#define ADDRESS_MISF_PICLOG_DATA_END    0x0015FFFF
#define ADDRESS_MISF_MEASUREMENT_START  0x00160000
#define ADDRESS_MISF_MEASUREMENT_END    0x0095FFFF

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




// ___________SMF-ADDRESS___________







#include "src/mmj_cigs_flash.c"

#endif // MMJ_CIGS_FLASH_H
//------------------End of File------------------
