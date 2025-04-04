#ifndef COMMUNICATION_H
#define COMMUNICATION_H

// fundamental function. It's format resieve signal to Command struct
Command make_receive_command(unsigned int8 receive_signal[], int8 receive_signal_size);
int8 make_frame(unsigned int8 receive_signal[], int8 receive_signal_size);
int8 get_content_size(unsigned int8 frame_id);
int1 check_crc(unsigned int8 frame[], int8 receive_frame_size);
unsigned int8 calc_crc(unsigned int8 frame[], int8 payload_size);
int1 check_device_id(unsigned int8 device_id);

// clear receive buffer
void clear_receive_signal(unsigned int8 receive_signal[], int8 *receive_signal_size);

// convet Command to byte and transmit this data
void transmit_command(int8 frame_id, unsigned int8 content[], int8 size);
void transmit(unsigned int8 data[], int8 data_size);

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



//__________ Receive signal max length _________
#define RECEIVE_SIGNAL_MAX 32

//__________ SFD (Start FrameDelimiter) _________

#define SFD 0xAA

//_____________ Device IDs ____________

#define MAIN_PIC  0x00
#define COM_PIC   0x01
#define RESET_PIC 0x02
#define FAB_PIC   0x03
#define BOSS_PIC  0x04
#define APRS_PIC  0x05
#define CAM_MCU   0x06
#define CHO_MCU   0x07
#define NAKA_PIC  0x08
#define SATO_PIC  0x09
#define BHU_MCU   0x0A
#define CIGS_PIC  0x0B

#define SELF_DEVICE_ID APRS_PIC


//____________ Frame IDs ____________
/* 
!!!NOTE!!!
 change write `value.h` line26
*/


// receives
#define UPLINK_COMMAND 0x00
#define STATUS_CHECK   0x01
#define IS_SMF_AVAILABLE 0x02

#define UPLINK_COMMAND_LENGTH 9
#define STATUS_CHECK_LENGTH   0
#define IS_SMF_AVAILABLE_LENGTH 1

#define RECEIVE_FRAME_KINDS 3


// transmits
#define MIS_MCU_STATUS 0x01
#define ACK 0x0F


//______________ Status ______________

// #define OFF       0x00
// #define BOOTING   0x01 these values used in BOSS PIC only
#define IDLE      0x02
#define BUSY      0x03
#define SMF_COPY_REQ 0x04
#define COPYING   0x05
#define FINISHED  0x06


//____________ Is SMF availavle respond ____________

#define ALLOW 0x00
#define DENY  0x01


#endif
