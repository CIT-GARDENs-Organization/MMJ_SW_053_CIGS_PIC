#include "timer.h"

int8 subsec;

#INT_TIMER1
void TIMER1_isr()
{
   set_timer1(TIMER_ISR_10MSEC); 
   tick_10ms++;
}

void timer_init()
{
   fprintf(PC, "Timer Initialize\r\n");
   clear_interrupt(INT_TIMER1);
   tick_10ms = 0;
   setup_timer_1(T1_EXTERNAL | T1_DIV_BY_1 | T1_ENABLE_SOSC);
   set_timer1(TIMER_ISR_10MSEC); 
   enable_interrupts(INT_TIMER1);   
   enable_interrupts(GLOBAL);
   fprintf(PC, "\tComplete\r\n");
}

void set_current_10msec(unsigned int32 new_10msec)
{
   tick_10ms = new_10msec;
}

unsigned int32 get_current_time_10ms()
{
   return tick_10ms;
}


unsigned int32 get_current_sec()
{
   return subsec;
}

unsigned int16 get_current_msec()
{
   // Assuming subsec is in deci-seconds (0.01 sec)
   return (subsec / 10); // Convert deci-seconds to milliseconds
}

unsigned int16 get_current_day()
{
   return day;
}


void add_current_msec(unsigned int32 add_msec)
{
   subsec += add_msec; // Convert milliseconds to deci-seconds
}
