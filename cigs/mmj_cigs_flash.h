#ifndef  MMJ_CIGS_FLASH_H
#define  MMJ_CIGS_FLASH_H




// MIS_FM Function
void misf_init(void);





#define PACKET_SIZE 64
#define MISF_DATA_HEADER_SIZE 64 // MISFデータヘッダーのサイズ

// __________MISF-ADDRESS__________
// DATA SIZE AREA
#define ADDRESS_DATA_SIZE_START 0x000000
#define ADDRESS_DATA_SIZE_END   0x000003
#define DATA_SIZE 4 // データサイズのサイズ
unsigned int8 data_size_loop_counter; // データサイズループカウンター
unsigned int32 data_size_counter; // データサイズカウンター


// PICLOG AREA
#define ADDRESS_PICLOG_START 0x000000
#define ADDRESS_PICLOG_END   0x000007
#define PICLOG_SIZE 6 // PICLOGのサイズ

typedef union {
    struct {
        unsigned int32 piclog_write_counter; // 時間
        unsigned int8 piclog_loop_counter; // 関数
        unsigned int32 piclog_smfwrite_counter; // パラメータ
        unsigned int8 piclog_smf_loop_counter; // SMF書き込みループカウンター
    }   fields; // フィールド
    unsigned int8 bytes[MISF_DATA_HEADER_SIZE]; // バイト配列
} PICLOG_DATA_HEADER; // PICLOGデータ




// MEASUREMENT DATA AREA
#define ADDRESS_MEASUREMENT_START ｓ
#define ADDRESS_MEASUREMENT_END   0x001FFF
#define MEASUREMENT_SIZE 16 // 測定データのサイズ
typedef union {
    struct {
        unsigned int32 measurement_data_counter;
        unsigned int8 meas_loop_counter;
        unsigned int32 measurement_data_smf_counter;
        unsigned int8 meas_smf_loop_counter;
    }   fields; // フィールド
    unsigned int8 bytes[MISF_DATA_HEADER_SIZE]; // バイト配列
} MEAS_DATA_HEADER; // PICLOGデータ




// ___________SMF-ADDRESS___________







#include "src/mmj_cigs_flash.c"

#endif // MMJ_CIGS_FLASH_H
//------------------End of File------------------
