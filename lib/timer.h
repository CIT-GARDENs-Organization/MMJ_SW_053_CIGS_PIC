#ifndef TIMER_H
#define TIMER_H

// __________ define _________

#define SEC_IN_A_DAY 86400


// __________ values _________

volatile unsigned int32 subsec = 0, sec = 0; //dsec -> deci second (0.01 sec)
volatile unsigned int16 day = 0;


// __________ functions _________
unsigned int32 get_current_sec();
unsigned int16 get_current_day();
void set_current_sec(unsigned int32 new_sec);

#INT_TIMER0
static void TIMER0_isr();


#include "./src/timer.c"

#endif

