#ifndef TYPEDEF_CONTENT_H
#define TYPEDEF_CONTENT_H

// there is used in main.c

typedef enum {
    EXECUTING_MISSION = 0x02,
    IDLE              = 0x03,
    SMF_USE_REQ       = 0x04,
    COPYING           = 0x05,
    FINISHED          = 0x06
} MisMcuStatusContent;


typedef enum {
    ALLOW = 0x00,
    DENY  = 0x01
} IsSmfAvailableContent;


#endif

// EOF
