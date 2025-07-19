#ifndef SMF_QUEUE_H
#define SMF_QUEUE_H



typedef enum {
   MEAURE_DATA,  // <- assign your missoins
   PICLOG_DATA
} MissionType;

typedef struct {
   unsigned int32 start_address;
   unsigned int32 end_address;
   int8 mission_flag;
} MissionTypeStruct;

typedef struct {
    MissionType   mission_type;
    unsigned int32 src;
    unsigned int32 size;
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

int1 is_empty_smf_data(void);

MissionTypeStruct getMissionTypeStruct(MissionType mis_type);

#include "smf_queue.c"

#endif

