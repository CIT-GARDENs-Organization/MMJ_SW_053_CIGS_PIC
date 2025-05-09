#INT_TIMER0
static void TIMER0_isr(void)
{
   // every 0.01 sec interrupts
   set_timer0(100);

   if (++subsec >= 100)
   {
      subsec = 0;
      sec++;

      if (sec >= SEC_IN_A_DAY)
      {
         sec -= SEC_IN_A_DAY;
         day++;
      }
   }
}

void initialize_timer()
{
   clear_interrupt(INT_TIMER1);
   setup_timer_1(T1_EXTERNAL | T1_DIV_BY_1); //Config TMR1, external clock, prescaler=1
   set_timer1(0x8000); //timer1 for 1 second interruption
   //T1OSCEN = 1; //Enable TMR1 Oscillator
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);
   fprintf(PC, "\tTimer initialize complete\r\n");
}
