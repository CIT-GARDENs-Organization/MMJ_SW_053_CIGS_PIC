#ifndef     MMJ_CIGS_PICLOG_H
#define     MMJ_CIGS_PICLOG_H

#define PICLOG_TIME_SIZE 4
#define PICLOG_TOTAL_SIZE 6 // Time + Function + Parameter

// PICLOG
typedef struct {
    unsigned int8 time[PICLOG_TIME_SIZE];
    unsigned int8 function;
    unsigned int8 parameter;
} PICLOG;



void piclog_init(void);
void piclog_make(unsigned int8 function, unsigned int8 parameter);
void piclog_read(unsigned int8 *data, unsigned int8 size);
void piclog_write(unsigned int8 *data, unsigned int8 size);



#include "../mmj_cigs_piclog.c"

#endif // MMJ_CIGS_PICLOG_H
//------------------End of File------------------
