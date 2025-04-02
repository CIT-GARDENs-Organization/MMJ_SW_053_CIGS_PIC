#ifndef DEFINE_H
#define DEFINE_H


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
