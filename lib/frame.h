#ifndef FRAME_CONTENT_H
#define FRAME_CONTENT_H


// __________ Receives _________

#define UPLINK_COMMAND        0x00
#define STATUS_CHECK          0x01
#define IS_SMF_AVAILABLE      0x02

#define UPLINK_COMMAND_LENGTH     9
#define STATUS_CHECK_LENGTH       0
#define IS_SMF_AVAILABLE_LENGTH   1

volatile FrameID frame_ids[] = {
   {UPLINK_COMMAND, UPLINK_COMMAND_LENGTH}, 
   {STATUS_CHECK, STATUS_CHECK_LENGTH}, 
   {IS_SMF_AVAILABLE, IS_SMF_AVAILABLE_LENGTH}
};


// __________ Transmits _____________

typedef enum {
   MIS_MCU_STATUS = 0x03,
   ACK = 0x0F
} TransmitFrameId;

#endif

