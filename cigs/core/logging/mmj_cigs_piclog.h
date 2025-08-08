#ifndef     MMJ_CIGS_PICLOG_H
#define     MMJ_CIGS_PICLOG_H

// PICLOG Param
#define PICLOG_STARTUP 0x00
#define PICLOG_PARAM_START 0x00
#define PICLOG_PARAM_END   0xFF


#define PICLOG_PACKET_SIZE 6

// PICLOG Union
typedef union {
    struct {
        unsigned int32 time;
        unsigned int8  function;
        unsigned int8  parameter;
    } fields;
    unsigned int8 bytes[PICLOG_PACKET_SIZE];
} PICLOG_t;

#define PICLOG_BLANK_SIZE 4
unsigned int8 PICLOG_BLANK_DATA[PICLOG_BLANK_SIZE];

// PICLOG 関数（実装は .c）
void piclog_make(unsigned int8 function, unsigned int8 parameter);
void piclog_write(unsigned int8 *data);

#endif // MMJ_CIGS_PICLOG_H
//------------------End of File------------------
