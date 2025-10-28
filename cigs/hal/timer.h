#ifndef _TIMER_H_
#define _TIMER_H_

// __________ define _________

#define SEC_IN_A_DAY 86400
#define CLOCK_FREQUENCY 32768 // 32.768 kHz

// __________ values _________

// Clock Freq : 32.768kHz
#define TIMER_ISR_1S 0x8000 // 1秒ごとのタイマー割り込み
#define TIMER_ISR_100MSEC 0xF313 // 100ミリ秒ごとのタイマー割り込み
#define TIMER_ISR_10MSEC 0xFEA8 // 10ミリ秒ごとのタイマー割り込み
#define TIMER_ISR_25MSEC 0xFC8D // 25ミリ秒ごとのタイマー割り込み

#INT_TIMER0
static void TIMER0_isr();
#INT_TIMER1 
static void TIMER1_isr();


// __________ functions _________
void timer_init();

unsigned int32 get_current_sec();
unsigned int16 get_current_msec();
void set_current_sec(unsigned int32 new_sec);


#endif // _TIMER_H_