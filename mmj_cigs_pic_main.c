#include "include/mmj_cigs_pic_main.h"

void main()
{ 
   fprintf(PC,"\r\n============================================================\r\n");
   fprintf(PC,"This is MOMIJI CIGS PIC BBM for MIS7_BBM1. Last updated on 2025/04/25, by Inoue.\r\n");  
   fprintf(PC,"See the changelog for the changes in this release\n\r");

   initialize_pic();
   initialize_uart();
   initialize_timer();
   //initialize_flash();


   int1 is_finished = FALSE;
   fprintf(PC,"CIGS PIC Start Operation\r\n\r\n");
   //Start loop
   while(!is_finished)
   {
      // handle from boss commands
      if(boss_receive_buffer_size > 0)
      {
         volatile Command command = make_receive_command(boss_receive_buffer, boss_receive_buffer_size);
         
         fprintf(PC, "FrameID: %1X\r\n", command.frame_id);
         fprintf(PC, "payload size: %d\r\n", command.size);
         fprintf(PC, "payload: ");
         for(int8 i = 0; i < command.size; i++)
            fprintf(PC, "%X ", command.content[i]);
         fprintf(PC, "\r\n\r\n");
         
         clear_receive_signal(boss_receive_buffer, &boss_receive_buffer_size);
            
         if(command.is_exist)
            is_finished = execute_command(&command);
         else
         {
            fprintf(PC, "\t-> Invalid command\r\n");
            continue;
         }
      }   // check if the command is valid
      
      // check mis mcu duration seconds (used in mission.c \ void continue_mis_mcu(int16 duration_sec))
      if (status == IDLE)
         if (duration_sec < sec && is_empty_smf_data())
            status = FINISHED;
      
      // check `is break while loop`
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

//
