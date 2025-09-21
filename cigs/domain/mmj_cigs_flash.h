#ifndef MMJ_CIGS_FLASH_H
#define MMJ_CIGS_FLASH_H

//#include <string.h>
#include "../hal/mmj_cigs_config.h"

#ifndef PACKET_SIZE
    #define PACKET_SIZE 64
#endif


#define MISF_START 0x00000000
#define MISF_END   0x00BC0FFF
#define SECTOR_64K_BYTE  0x10000

#define MISF_CIGS_DATA_TABLE_SIZE 64

// Flash Address
#define MISF_CIGS_DATA_TABLE_START 0x00000000
#define MISF_CIGS_DATA_TABLE_END   0x00000FFF
#define MISF_CIGS_DATA_TABLE_SIZE  4096
#define MISF_CIGS_PICLOG_START    0x00010000
#define MISF_CIGS_PICLOG_END      0x00140FFF
#define MISF_CIGS_PICLOG_SIZE     1310720
#define MISF_CIGS_ENVIRO_START    0x00281000
#define MISF_CIGS_ENVIRO_END      0x00320FFF
#define MISF_CIGS_ENVIRO_SIZE     1310720
#define MISF_CIGS_IV1_HEADER_START 0x00721000
#define MISF_CIGS_IV1_HEADER_END   0x007C0FFF
#define MISF_CIGS_IV1_HEADER_SIZE  655360
#define MISF_CIGS_IV1_DATA_START   0x007C1000
#define MISF_CIGS_IV1_DATA_END     0x00BC0FFF
#define MISF_CIGS_IV1_DATA_SIZE    4194304
#define MISF_CIGS_IV2_HEADER_START 0x00721000
#define MISF_CIGS_IV2_HEADER_END   0x007C0FFF
#define MISF_CIGS_IV2_HEADER_SIZE  655360
#define MISF_CIGS_IV2_DATA_START   0x007C1000
#define MISF_CIGS_IV2_DATA_END     0x00BC0FFF
#define MISF_CIGS_IV2_DATA_SIZE    4194304

// 重複していた #define はここに一度だけ残す
// 例: #define SECTOR_4K_BYTE  0x1000  // 4KByte
// 他ファイルに同じ行があれば削除

// Flash カウンタだけを保持する汎用構造
typedef struct {
    unsigned int32 used_counter;
    unsigned int32 uncopied_counter;
    unsigned int8  reserve_counter1;
    unsigned int8  reserve_counter2;
} FlashCounter_t;

// データ種別ID
typedef enum {
    FLASH_ID_DATA_TABLE = 0 ,
    FLASH_ID_PICLOG ,
    FLASH_ID_ENVIRONMENT,
    FLASH_ID_IV1_HEADER,
    FLASH_ID_IV1_DATA,
    FLASH_ID_IV2_HEADER,
    FLASH_ID_IV2_DATA,
    FLASH_ID_COUNT = 7
} FlashDataId_t;

// Flash_t : 実体側で使用する拡張(必要なら id を保持)
typedef struct {
    FlashDataId_t   id;
    unsigned int32  used_counter;
    unsigned int32  uncopied_counter;
    unsigned int8   reserve_counter1;
    unsigned int8   reserve_counter2;
} Flash_t;

// FlashData_t （既存定義が別ヘッダなら重複しないよう確認）
// ここでは payload 内の logdata.* が FlashCounter_t と仮定
typedef union {
    struct {
        union {
            struct {
                FlashCounter_t piclog;
                FlashCounter_t environment;
                FlashCounter_t iv1_header;
                FlashCounter_t iv1_data;
                FlashCounter_t iv2_header;
                FlashCounter_t iv2_data;
            } logdata;  // 合計サイズは63バイト
            unsigned int8 reserve[63];
        } payload;
        unsigned int8 crc;  // 1バイトCRC
    } packet;
    unsigned int8 bytes[64];
} FlashData_t;

// グローバルカウンタ（他 .c で定義済みなら extern に）
extern Flash_t *data_table_ptr;
extern Flash_t *piclog_data_ptr;
extern Flash_t *environment_data_ptr;
extern Flash_t *iv1_header_ptr;
extern Flash_t *iv1_data_ptr;
extern Flash_t *iv2_header_ptr;
extern Flash_t *iv2_data_ptr;

// 取得系
FlashData_t make_flash_data_table(void);
void misf_update_address_area(void);
void print_flash_status(void);

// MISF アドレス/書き込み構造体
typedef struct {
    int32 start_address;
    int32 end_address;
} MisfAddressStruct;

typedef struct {
    int32 start_address;
    int32 size;
} MisfWriteStruct;

MisfAddressStruct get_misf_address_struct(MissionID mission_id);
MisfWriteStruct  get_misf_write_struct(MissionID mission_id);

// キュー投入
void add_smf_queue(MissionID mission_id, FunctionType func_type, SmfWriteMode write_mode);
void misf_init();

typedef struct {
    unsigned int32 start;
    unsigned int32 end;
    unsigned int32 size;
} ADDRESS_AREA_T;


extern const ADDRESS_AREA_T MISF_ADDRESS_TABLE[];
extern FlashCounter_t flash_counter_table[];
extern FlashCounter_t *flash_counter_table_ptr[];

void print_MISF_ADDRESS_TABLE(void);

void misf_write_data(FlashDataId_t id, int8 *data_ptr, unsigned int16 size);
#endif
//------------------End of File------------------
