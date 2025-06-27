#INT_TIMER1
void TIMER1_isr()
{
   set_timer1(2^16-CLOCK_FREQUENCY); 
   sec++;
   /*
   if (++subsec >= 100)
   {
      subsec = 0;
      sec++;

      if (sec >= SEC_IN_A_DAY)
      {
         day++;
      }
   }*/
}

void setup_timer()
{
   fprintf(PC, "Timer Initialize\r\n");
   clear_interrupt(INT_TIMER1);
   //sec = 0;
   // 外部クロックをT1CKIピンから入力、プリスケーラなし
   setup_timer_1(T1_EXTERNAL | T1_DIV_BY_1 | T1_ENABLE_SOSC);
   set_timer1(2^16-CLOCK_FREQUENCY); 
   //set_timer1(0);  // タイマーカウント初期値設定
   //T1OSCEN = 1; //Enable TMR1 Oscillator
   enable_interrupts(INT_TIMER1);   
   enable_interrupts(GLOBAL);
   fprintf(PC, "\tComplete\r\n");
}

void set_current_sec(unsigned int32 new_sec)
{
   sec = new_sec;
}


unsigned int32 get_current_sec()
{
   return sec;
}

unsigned int16 get_current_day()
{
   return day;
}


