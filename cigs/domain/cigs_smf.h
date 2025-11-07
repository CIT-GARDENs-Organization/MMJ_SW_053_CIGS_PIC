#ifndef MMJ_CIGS_SMF_H
#define MMJ_CIGS_SMF_H

#include "../lib/tool/smf_queue.h"  // FlashOperationStruct定義
#include "../hal/cigs_config.h" // int8,int32 型など(CCS C) 依存
#include "cigs_flash.h"       // MISF_ADDRESS_TABLE, misf_counter_table 依存

#define CRC_RETRY_COUNT 5

extern const ADDRESS_AREA_T MISF_ADDRESS_TABLE[];
extern FlashCounter_t smf_counter_table[];

extern int1 smf_update_flag[10]; // SMF更新フラグ


// SMF管理用構造体
typedef struct {
    ADDRESS_AREA_T header_area;
    ADDRESS_AREA_T data_area;
} SMF_AREA_T;
#define SMF_ID_COUNT 7


// Mission ID 列挙（smf_queue.h の MissionID と整合させる）
typedef enum{
    SMF_MISSION_DATA_TABLE = CIGS_DATA_TABLE,
    SMF_MISSION_PICLOG     = CIGS_PICLOG_DATA,
    SMF_MISSION_ENVIRO     = CIGS_ENVIRO_DATA,
    SMF_MISSION_IV1_HEADER = CIGS_IV1_HEADER,
    SMF_MISSION_IV1_DATA   = CIGS_IV1_DATA,
    SMF_MISSION_IV2_HEADER = CIGS_IV2_HEADER,
    SMF_MISSION_IV2_DATA   = CIGS_IV2_DATA
} SmfMissionAlias; // 利便用Alias

// SMF操作関数
void smf_write(FlashOperationStruct *smf_data_ptr);
void smf_read(FlashOperationStruct *smf_data_ptr);
void smf_erase(FlashOperationStruct *smf_data_ptr);

// カウンター管理関数
void update_misf_counters(int8 mission_id, int32 transfer_size);
void reset_misf_counters(int8 mission_id);
void print_misf_counter_status(int8 mission_id);


// SMF Partition Header
typedef struct {
    int32 misf_used_size;
    int32 smf_copied_size;
} SMF_PARTITION;




typedef union {
    unsigned int8 bytes[PACKET_SIZE];  // 生データアクセス用
    struct {
        SMF_PARTITION headers[7];
        unsigned int8 reserved[7];
        unsigned int8 crc;  // 最後の1バイト
    } fields;
} smf_data_table_t;

void smf_write_header(smf_data_table_t *smf_data_table);
void print_smf_counter_status(smf_data_table_t *smf_data_table);

// mission_idから対応するSMF_PARTITIONを取得する関数
SMF_PARTITION* get_smf_partition_by_mission_id(int8 mission_id);
void update_smf_partition_by_mission_id(int8 mission_id, int32 used_size, int32 loop_counter);

void smf_update_tablearea(void);

typedef union{
    unsigned int8 bytes[PACKET_SIZE];
    struct {
        SMF_PARTITION partitions[FLASH_ID_COUNT];
        unsigned int8 reserved[PACKET_SIZE - (sizeof(SMF_PARTITION) * FLASH_ID_COUNT) - 1];
        unsigned int8 crc;
    } fields;
}smf_address_header_t;



#endif  // MMJ_CIGS_SMF_H