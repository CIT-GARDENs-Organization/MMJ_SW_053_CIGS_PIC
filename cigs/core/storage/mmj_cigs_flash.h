#ifndef  MMJ_CIGS_FLASH_H
#define  MMJ_CIGS_FLASH_H




// MIS_FM Function
void misf_init(void);
void update_misf_data_header(void);
void write_misf_address_area(void);
void init_cigs_counters(void);
void print_counter_status(void);



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

// Counter Union (64 bytes)
typedef union {
    struct {
        // SMF Counters (16 bytes)
        int32  smf_piclog_use_counter;     // 4 bytes
        int8   smf_piclog_loop_counter;    // 1 byte
        int32  smf_meas_use_counter;       // 4 bytes
        int8   smf_meas_loop_counter;      // 1 byte
        int8   smf_reserved1[6];           // 6 bytes padding
        
        // MISF Counters (32 bytes)
        int32  misf_piclog_use_counter;    // 4 bytes
        int8   misf_piclog_loop_counter;   // 1 byte
        int32  misf_piclog_uncopyed_counter; // 4 bytes
        int8   misf_piclog_write_counter;  // 1 byte
        int32  misf_meas_use_counter;      // 4 bytes
        int8   misf_meas_loop_counter;     // 1 byte
        int32  misf_meas_uncopyed_counter; // 4 bytes
        int8   misf_reserved2[13];         // 13 bytes padding
        
        // Reserved for future use (16 bytes)
        int8   reserved3[16];
    } counters;
    
    // Raw byte access
    int8 raw_data[64];
} CounterUnion;

// Global counter instance
extern CounterUnion cigs_counters;

// マクロでアクセスを簡単にする
#define smf_piclog_use_counter      cigs_counters.counters.smf_piclog_use_counter
#define smf_piclog_loop_counter     cigs_counters.counters.smf_piclog_loop_counter
#define smf_meas_use_counter        cigs_counters.counters.smf_meas_use_counter
#define smf_meas_loop_counter       cigs_counters.counters.smf_meas_loop_counter
#define misf_piclog_use_counter     cigs_counters.counters.misf_piclog_use_counter
#define misf_piclog_loop_counter    cigs_counters.counters.misf_piclog_loop_counter
#define misf_piclog_uncopyed_counter cigs_counters.counters.misf_piclog_uncopyed_counter
#define misf_piclog_write_counter   cigs_counters.counters.misf_piclog_write_counter
#define misf_meas_use_counter       cigs_counters.counters.misf_meas_use_counter
#define misf_meas_loop_counter      cigs_counters.counters.misf_meas_loop_counter
#define misf_meas_uncopyed_counter  cigs_counters.counters.misf_meas_uncopyed_counter





#define DATA_HEADER_SIZE 64 

typedef enum{
    MISF_DATA_TABLE = 0x00, // CIGSデータテーブル
    MISF_PICLOG = 0x01, // CIGSピクログヘッダー
    MISF_MEAS_HEADER = 0x02, // CIGS測定データヘッダー
    MISF_MEAS_DATA = 0x03 // CIGS測定データ
} MissionTypeEnum;


// ___________SMF-ADDRESS___________









#endif // MMJ_CIGS_FLASH_H
//------------------End of File------------------
