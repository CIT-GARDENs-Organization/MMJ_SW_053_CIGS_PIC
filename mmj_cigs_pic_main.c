#include "mmj_cigs_pic_main.h"

//____________ intrrupts ____________
#INT_TIMER0
void TIMER0_isr(void) 
{
   // every 0.1 sec interrupts
   set_timer0(100);

   if (++dsec >= 100)
   {
      dsec = 0;
      sec++;

      if (sec >= 86400)
      {
         sec -= 86400;
         day++;
      }
   }
}

#INT_RDA
void RDA_isr(void)
{
   if (status == BUSY || status == COPYING)
   {
      fgetc(BOSS);
      fputc(0xAA, BOSS);
   }
   else
   {
      if (receive_signal_size < 16)
         receive_signal[receive_signal_size++] = fgetc(BOSS);
      else
         fgetc(BOSS);
   }
}



//____________ initialize ____________

void initialize(void)
{
   fprintf(PC, "Start setting\r\n");
   setup_lcd(LCD_DISABLED);
   setup_timer_0(T0_INTERNAL | T0_DIV_256 | RTCC_8_BIT);
   enable_interrupts(INT_RDA);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);
   fprintf(PC, "End setting\r\n");
}


// ___________ command execution ___________

int1 execute_command(Command *command)
{
   fprintf(PC, "\r\nStart execute_command\r\n");
   
   switch(command->frame_id)
   {
      case UPLINK_COMMAND:
         fprintf(PC, "\t-> Uplink command\r\n");
         fprintf(PC, "\t   Transmit Acknolegde\r\n");
         transmit_command(ACK, 0, 0);

         status = BUSY;
         execute_mission(command->content);
         if (!is_empty_smf_data())
            status = SMF_COPY_REQ;
         else
            status = FINISHED;
         break;
      
      case STATUS_CHECK:
         fprintf(PC, "\t-> Status check\r\n");
         fprintf(PC, "\t\t-> My status is %d\r\n", status);
         fprintf(PC, "\t   Transmit MIS MCU Status\r\n");
         transmit_command(MIS_MCU_STATUS, &status, 1);
         if (status == FINISHED)
         {
            fprintf(PC, "finished in status_check\r\n");
            return TRUE;
         }
         break;
      
      case IS_SMF_AVAILABLE:
         fprintf(PC, "\t-> is SMF available\r\n");
         fprintf(PC, "\t   Transmit Acknolegde\r\n");
         transmit_command(ACK, 0, 0);
         if (command->content[0] == ALLOW)
         {
            fprintf(PC, "\t\t-> allowd\r\n");
            status = COPYING;
            copy_data();
            status = FINISHED;
         }
         else
            fprintf(PC, "\t\t-> denyed\r\n");
   }
   return FALSE;
}


// ___________ main functoin _______________

void main()
{
   fprintf(PC, "\r\n\r\n");
   for (int8 i = 0; i < 3; i++)
   {
      for (int8 j = 0; j < 31; j++)
         fprintf(PC, "_");
      fprintf(PC, "\r\n");
   }
         
   fprintf(PC, "___________Start main__________\r\n\r\n");
   
   initialize();
   
   int1 is_finished = FALSE;

   //Start loop
   while(!is_finished)
   {
      //receive anything signal
      if(receive_signal_size > 0)
      {
         Command command = make_receive_command(receive_signal, receive_signal_size);
         clear_receive_signal(receive_signal, &receive_signal_size);
            
         if(command.is_exist)
            is_finished = execute_command(&command); 
      }
      
      if(is_finished == TRUE)
         break;
         
      delay_ms(400);
   }
   
   fprintf(PC, "\r\n\r\n======\r\n\r\nFinished process.\r\nWait for BOSS PIC turn off me");
   
   while (TRUE)
   {
      fprintf(PC, ".");
      delay_ms(1000);
   }
   
   fprintf(PC, "End main\r\n");
}
