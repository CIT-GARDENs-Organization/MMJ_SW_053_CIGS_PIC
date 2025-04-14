#ifndef FUNCTION_H
#define FUNCTION_H


# define MODE_UPDATETIME 0x00
// timer
volatile unsigned int32 dsec = 0, sec = 0; //dsec -> deci second (0.1 sec)

//volatile unsigned int16 day = 0;

void update_time(unsigned int8 raw_time[]);
void checksum(void);


#endif  // FUNCTION_H
