#ifndef     MMJ_CIGS_PICLOG_H
#define     MMJ_CIGS_PICLOG_H


// PICLOG
typedef union {
    struct {
        unsigned int32 time;
        unsigned int8 function;
        unsigned int8 parameter;
    };
    unsigned int8 bytes[6];  
} PICLOG;

// extern宣言を追加（実体は.cファイルで定義）


void piclog_make(unsigned int8 function, unsigned int8 parameter);
void piclog_write(unsigned int8 *data);

#define PICLOG_PACKET_SIZE = 60 // PICLOGのパケットサイズ


#include "src/mmj_cigs_piclog.c"

#endif // MMJ_CIGS_PICLOG_H
//------------------End of File------------------
