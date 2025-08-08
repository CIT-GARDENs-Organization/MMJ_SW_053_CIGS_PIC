#ifndef  MMJ_CIGS_FLASH_H
#define  MMJ_CIGS_FLASH_H




// MIS_FM Function
void misf_init(void);
void update_misf_data_header(void);
void write_misf_address_area(void);
void read_misf_address_area(unsigned int8 *data);


#define PACKET_SIZE 64
#define MISF_DATA_HEADER_SIZE 64 // MISFデータヘッダーのサイズ
#define MISF_PICLOG_MAX_COUNT 60




// __________MISF-ADDRESS__________
#define ADDRESS_MISF_START              0x00000000
#define ADDRESS_MISF_END                0x00F42400
#define ADDRESS_MANAGE_START            0x00000000
#define ADDRESS_MANAGE_END              0x0000FFFF
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



// Flashに関する構造体。基本的にこれを用いる
typedef struct {
    enum{} id;
    unsigned int32 used_counter;
    unsigned int32 uncopied_counter;
    unsigned int8 reserve_counter1;
    unsigned int8 reserve_counter2;
} Flash_t;

// Flash_t instances
Flash_t piclog_data;
Flash_t environment_data;
Flash_t iv_header;
Flash_t iv_data;

// 各データブロックのカウンタ情報
typedef struct __attribute__((packed)) {
    unsigned int32 used_counter;
    unsigned int32 uncopied_counter;
    unsigned int8 reserve_counter1;
    unsigned int8 reserve_counter2;
} FlashCounter_t;

// union定義
typedef union __attribute__((packed)) {
    unsigned int8 bytes[PACKET_SIZE];
    struct {
        union {
            unsigned int8 raw[PACKET_SIZE - 2];

            struct __attribute__((packed)) {
                FlashCounter_t piclog;
                FlashCounter_t environment;
                FlashCounter_t iv_header;
                FlashCounter_t iv_data;
                // 合計サイズ: (4+4+1+1)*4 = 40 バイト
            } logdata;

        } payload;
        unsigned int8 crc;
    } packet;
} FlashData_t;











#define DATA_HEADER_SIZE 64 




// ___________SMF-ADDRESS___________









#endif // MMJ_CIGS_FLASH_H
//------------------End of File------------------
