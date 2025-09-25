#include "timer.h"

int8 subsec;
unsigned int16 time_ms = 0;
unsigned int32 time_sec = 0;

#INT_TIMER0
void TIMER0_isr()
{
   set_timer0(0xEC78);  // 10msごとにリロード
   time_ms += 10;
   if (time_ms >= 1000) {
      time_ms = 0;
   }
}

#INT_TIMER1 
void TIMER1_isr()
{
   set_timer1(TIMER_ISR_1S); 
   time_ms += 1000;
   if (time_ms >= 1000) {
      time_ms = 0;
      time_sec++;
   }
}

void timer_init()
{
   fprintf(PC, "Timer Initialize\r\n");
   clear_interrupt(INT_TIMER1);
   time_ms = 0;
   time_sec = 0;
   setup_timer_1(T1_EXTERNAL | T1_DIV_BY_1 | T1_ENABLE_SOSC);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_64|RTCC_8_BIT);
   set_timer1(TIMER_ISR_1S); 
   enable_interrupts(INT_TIMER1);   
   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);
   fprintf(PC, "\tComplete\r\n");
}

void set_current_sec(unsigned int32 new_sec)
{
   time_sec = new_sec;
}

unsigned int32 get_current_sec()
{
   return time_sec;
}

unsigned int16 get_current_msec()
{
   return time_ms;
}

