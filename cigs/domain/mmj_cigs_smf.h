#ifndef MMJ_CIGS_SMF_H
#define MMJ_CIGS_SMF_H

#include "../lib/tool/smf_queue.h"  // FlashOperationStruct定義
#include "../hal/mmj_cigs_config.h" // int8,int32 型など(CCS C) 依存

#define CRC_RETRY_COUNT 5

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
    int32 used_size;
    int32 loop_counter;
    int8 reserved[3];
} SMF_PARTITION;

// パーティション管理構造体
typedef struct {
    SMF_PARTITION piclog;     // データテーブル領域は利用しないため削除
    SMF_PARTITION env;
    SMF_PARTITION iv1_header;
    SMF_PARTITION iv1_data;
    SMF_PARTITION iv2_header;
    SMF_PARTITION iv2_data;
    int32 write_bytes[4];
} PartitionParam;

extern PartitionParam param;  // extern宣言に変更


// MSB形式でpartition_headerに値を設定する関数（piclog等必要分のみ）
void set_piclog_used_size_msb(int32 value);
int32 get_piclog_used_size_msb(void);
// 旧 data_table/meas 互換マクロは削除（不要）

// mission_idから対応するSMF_PARTITIONを取得する関数
SMF_PARTITION* get_smf_partition_by_mission_id(int8 mission_id);
void update_smf_partition_by_mission_id(int8 mission_id, int32 used_size, int32 loop_counter);


#endif  // MMJ_CIGS_SMF_H