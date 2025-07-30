#ifndef MMJ_CIGS_SMF_H
#define MMJ_CIGS_SMF_H

#include "mmj_cigs_smf.h"

#define PACKET_SIZE 64  // パケットサイズ定義



void smf_write(SmfDataStruct *smf_data);
void smf_read(SmfDataStruct *smf_data);
void smf_erase(SmfDataStruct *smf_data);



// SMF Partition Header
typedef struct {
    int32 used_size;
    int32 loop_counter;
    int8 reserved[3];
} SMF_PARTITION;

//
typedef struct {
    SMF_PARTITION meas;
    SMF_PARTITION piclog;
    int32 write_bytes[4];
} PartitionParam;

PartitionParam param = {0};


// MSB形式でpartition_headerに値を設定する関数
void set_meas_used_size_msb(int32 value);
void set_piclog_used_size_msb(int32 value);
int32 get_meas_used_size_msb(void);
int32 get_piclog_used_size_msb(void);

// mission_idから対応するSMF_PARTITIONを取得する関数
SMF_PARTITION* get_smf_partition_by_mission_id(int8 mission_id);
void update_smf_partition_by_mission_id(int8 mission_id, int32 used_size, int32 loop_counter);


#endif  // MMJ_CIGS_SMF_H
