#ifndef TIMER_H
#define TIMER_H

// __________ define _________

#define SEC_IN_A_DAY 86400
#define CLOCK_FREQUENCY 32768 // 32.768 kHz

// __________ values _________

int32 tick_10ms = 0; 

unsigned int32 dsec = 0, sec = 0; //dsec -> deci second (0.01 sec)
volatile unsigned int16 day = 0;

void timer_init();
// __________ functions _________
unsigned int32 get_current_sec();
unsigned int16 get_current_msec();
unsigned int16 get_current_day();
void set_current_10msec(unsigned int32 new_10msec);

#INT_TIMER0
static void TIMER0_isr();

#INT_TIMER1 
static void TIMER1_isr();

// Clock Freq : 32.768kHz
#define TIMER_ISR_1S 0x8000 // 1秒ごとのタイマー割り込み
#define TIMER_ISR_100MSEC 0xF313 // 100ミリ秒ごとのタイマー割り込み
#define TIMER_ISR_10MSEC 0xFEA8 // 10ミリ秒ごとのタイマー割り込み
#define TIMER_ISR_25MSEC 0xFC8D // 25ミリ秒ごとのタイマー割り込み

void set_current_sec(unsigned int32 new_sec);
unsigned int32 get_current_sec();
unsigned int16 get_current_msec();


#endif

