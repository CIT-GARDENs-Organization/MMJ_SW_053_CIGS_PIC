#ifndef VALUE_H
#define VALUE_H


// timer
volatile unsigned int32 dsec = 0, sec = 0; //dsec -> deci second (0.1 sec)
volatile unsigned int16 day = 0;


// receive buffer
volatile unsigned int8 receive_signal[RECEIVE_SIGNAL_MAX] = {0x00};
volatile int8 receive_signal_size = 0;


// MIS MCU statement
volatile int8 status = IDLE;


// for use ID and length data table
typedef struct{
   int8 id;
   int8 length;
   unsigned int8 name[32];
}FrameID;

const FrameID frame_ids[RECEIVE_FRAME_KINDS] = {
   {UPLINK_COMMAND, UPLINK_COMMAND_LENGTH, "Uplink commnd"}, 
   {STATUS_CHECK, STATUS_CHECK_LENGTH, "Status check"}, 
   {IS_SMF_AVAILABLE, IS_SMF_AVAILABLE_LENGTH, "Is SMF available"}
};


// Command struct
typedef struct{
   int8 frame_id;
   unsigned int8 content[RECEIVE_SIGNAL_MAX];
   int8 size;
   int1 is_exist;
} Command;


// SMF data queue
typedef struct{
   unsigned int32 src;
   unsigned int32 dest;
   unsigned int32 size;
}SmfDataStruct;

#define SMF_DATA_SIZE 16

volatile int8 smf_data_head = 0;
volatile int8 smf_data_tail = 0;

SmfDataStruct smf_data[SMF_DATA_SIZE] = { {0x00} };


#endif  // VALUE_H
