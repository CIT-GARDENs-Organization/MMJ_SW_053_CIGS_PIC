#ifndef MMJ_CIGS_FLASH_H
#define MMJ_CIGS_FLASH_H

#include <string.h>
#include "../../system/mmj_cigs_config.h"

#define MISF_START 0x00000000
#define MISF_END   0x000FFFFF
#define SECTOR_64K_BYTE  0x10000

#define MISF_CIGS_DATA_TABLE_SIZE 64


#define MISF_CIGS_DATA_TABLE_START 0x00000000
#define MISF_CIGS_DATA_TABLE_END   0x0000FFFF
#define MISF_CIGS_PICLOG_START    0x00010000
#define MISF_CIGS_PICLOG_END      0x0001FFFF
#define MISF_CIGS_ENVIRO_START    0x00020000
#define MISF_CIGS_ENVIRO_END      0x0002FFFF
#define MISF_CIGS_IV_HEADER_START 0x00030000
#define MISF_CIGS_IV_HEADER_END   0x0003FFFF
#define MISF_CIGS_IV_DATA_START   0x00040000
#define MISF_CIGS_IV_DATA_END     0x0004FFFF

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
    FLASH_ID_PICLOG = 0,
    FLASH_ID_ENVIRONMENT,
    FLASH_ID_IV_HEADER,
    FLASH_ID_IV_DATA,
    FLASH_ID_UNKNOWN = 0x7F
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
        struct {
            struct {
                FlashCounter_t piclog;
                FlashCounter_t environment;
                FlashCounter_t iv_header;
                FlashCounter_t iv_data;
            } logdata;
        } payload;
        unsigned int8 crc;
    } packet;
    unsigned int8 bytes[PACKET_SIZE];
} FlashData_t;

// グローバルカウンタ（他 .c で定義済みなら extern に）
Flash_t piclog_data;
Flash_t environment_data;
Flash_t iv_header;
Flash_t iv_data;

// 取得系
FlashData_t make_flash_data_table(void);
void write_misf_address_area(void);
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

#endif
//------------------End of File------------------
