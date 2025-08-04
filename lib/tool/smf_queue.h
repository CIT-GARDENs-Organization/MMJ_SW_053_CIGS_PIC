#ifndef SMF_QUEUE_H
#define SMF_QUEUE_H

typedef enum {
   SMF_WRITE = 0x00,    // SMF書き込み操作
   SMF_READ = 0x01,     // SMF読み込み操作
   SMF_ERASE = 0x02     // SMF消去操作
} FunctionType;

/*
 * 使用例:
 * SmfDataStruct write_data = {SMF_WRITE, 0x1000, 256};
 * SmfDataStruct read_data = {SMF_READ, 0x2000, 512};
 * SmfDataStruct erase_data = {SMF_ERASE, 0x3000, 1024};
 */

typedef struct {
   unsigned int32 start_address;
   unsigned int32 end_address;
} MissionTypeStruct;

typedef struct {
    int8 mission_id; 
    int32 src;
    int32 size;
    FunctionType   func_type;
} SmfDataStruct;

#define SMF_DATA_SIZE 16
typedef struct {
   SmfDataStruct smf_data[SMF_DATA_SIZE];
   int8 smf_data_head;
   int8 smf_data_tail;
} SmfQueueStruct;

// _____________________ values _________________

volatile SmfQueueStruct smf_queue = {0};


// ___________________ functions ________________


void enqueue_smf_data(SmfDataStruct *data);

SmfDataStruct *dequeue_smf_data();

SmfDataStruct *peek_smf_data(void);

void remove_smf_data(void);

int1 is_empty_smf_data(void);

MissionTypeStruct getMissionTypeStruct(FunctionType func_type);

#endif

