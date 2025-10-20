#include "mmj_cigs_main.h"  // ルート直下のヘッダー

void main()
{ 
   delay_ms(100); // wait for power stable
   fprintf(PC,"\r\n\r\n\r\n============================================================\r\n");
   fprintf(PC,"This is MOMIJI CIGS PIC BBM for MIS7_BBM6.\r\n");
   fprintf(PC,"Last updated on %s %s, by Inoue.\r\n\r\n", __DATE__, __TIME__);
   memset(smf_update_flag, 0, sizeof(smf_update_flag));
   // io_init();
   setup_uart_to_boss();
   timer_init();
   ad7490_init();
   mcp4901_init();
   misf_init();
   
   piclog_make(PICLOG_STARTUP,0x00);

   int1 is_finished = FALSE;
   fprintf(PC,"____CIGS PIC Start Operation_____\r\n\r\n");
   
   //enable_negative_power();
   connect_port1();
   connect_port2();
   mcp4901_1_write(0);
   mcp4901_2_write(0);
   connect_negative_power1(TRUE); // CIGS1 Negative Power ON
   
   fprintf(PC,"waiting for BOSS PIC command");

   //Start loop
   while(TRUE)
   {
      if(boss_receive_buffer_size > 0)
      {
         //Command command = make_command(boss_receive_buffer, boss_receive_buffer_size);
         volatile Command recieve_cmd = make_receive_command(boss_receive_buffer, boss_receive_buffer_size);
         clear_receive_signal(boss_receive_buffer, &boss_receive_buffer_size);
         
         if(recieve_cmd.is_exist){
            is_finished = execute_command(&recieve_cmd); // FIX: 戻り値を保持
            if (is_finished == TRUE)
            {
               fprintf(PC, "\r\n\r\n======\r\n\r\nFinished process.\r\n");
               

            }else{
               fprintf(PC,"\r\nwaiting for BOSS PIC command");
            }
         }
      }
         
      delay_ms(500);
      fprintf(PC, ".");
   }
   
   fprintf(PC, "End main\r\n");
   
}
// End of files
