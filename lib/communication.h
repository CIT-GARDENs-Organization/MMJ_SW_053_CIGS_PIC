#ifndef COMMUNICATION_H
#define COMMUNICATION_H

// __________ device IDs ______________

#define GS        0x00
#define MAIN_PIC  0x01
#define COM_PIC   0x02
#define RESET_PIC 0x03
#define FAB_PIC   0x04
#define BOSS_PIC  0x05
#define APRS_PIC  0x06
#define CAM_MCU   0x07
#define CHO_MCU   0x08
#define NAKA_PIC  0x09
#define SATO_PIC  0x0A
#define BHU_MCU   0x0B
#define CIGS_PIC  0x0C

#define SELF_DEVICE_ID CIGS_PIC // <- change to your device name












// ____________ SFD ______________

#define SFD 0xAA


// ____________ typedef _____________

typedef struct {
    int8 id;
    int8 length;
} FrameID;

#define CONTENT_MAX 9
typedef struct {
    int8 frame_id;
    unsigned int8 size;
    int1 is_exist;
    unsigned int8 content[CONTENT_MAX];
} Command;

#include "./frame.h"

// ______ Receive _______

Command make_receive_command(unsigned int8 receive_signal[], int8 receive_signal_size);

static int8 trim_receive_signal_header(unsigned int8 receive_signal[], int8 receive_signal_size);

static int8 get_content_size(unsigned int8 frame_id);

static int1 check_crc(unsigned int8 frame[], int8 receive_frame_size);

static int1 check_device_id(unsigned int8 device_id);


// ______ Transmit _______

void transmit_command(TransmitFrameId frame_id, unsigned int8 content[], int8 size);

static void transmit(unsigned int8 data[], int8 data_size);

// ______ Common _________

static unsigned int8 calc_crc8(unsigned int8 frame[], int8 payload_size);

#include "./src/communication.c"

#endif

