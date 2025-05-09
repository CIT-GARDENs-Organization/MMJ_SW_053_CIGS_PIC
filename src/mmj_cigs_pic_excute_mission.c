#include "mmj_cigs_pic_mode.h"

#separate
int1 execute_command(Command *command)
{
   fprintf(PC, "Start execute_command\r\n");

   switch (command->frame_id)
   {
      case UPLINK_COMMAND:
         return handle_uplink_command(command);
      
      case STATUS_CHECK:
         return handle_status_check(command);
      
      case IS_SMF_AVAILABLE:
         return handle_smf_available(command);
   }

   return FALSE;
}


// ___________ command execution ___________
int1 handle_uplink_command(Command *command)
{
   fprintf(PC, "\t-> Uplink command\r\n");
   fprintf(PC, "\t   Transmit Acknowledge\r\n");
   transmit_ack();

   status = EXECUTING_MISSION;
   execute_mission(command->content);

   if (is_empty_smf_data())
   {
      if (duration_sec < sec)
         status = FINISHED;
      else
         status = IDLE;
   }
   else
   {
      status = SMF_USE_REQ;
   }

   return FALSE;
}


int1 handle_status_check(Command *command)
{
   fprintf(PC, "\t-> Status check\r\n");
   fprintf(PC, "\t\t-> My status is %d\r\n", status);
   fprintf(PC, "\t   Transmit MIS MCU Status\r\n");
   transmit_status();

   if (status == FINISHED)
   {
      fprintf(PC, "finished in status_check\r\n");
      return TRUE;
   }

   return FALSE;
}

int1 handle_smf_available(Command *command)
{
   fprintf(PC, "\t-> is SMF available\r\n");
   fprintf(PC, "\t   Transmit Acknowledge\r\n");
   transmit_ack();

   if (command->content[0] == ALLOW)
   {
      fprintf(PC, "\t\t-> allowed\r\n");
      status = COPYING;
      copy_data();
      status = FINISHED;
   }
   else
   {
      fprintf(PC, "\t\t-> denied\r\n");
   }

   return FALSE;
}

void execute_mission(unsigned int8 *content)
{
   // record to executed mission list
   unsigned int8 command_id = content[0];
   unsigned int8 *parameter = &content[1];
   
   if (is_executed_mission(command_id))
   {
      fprintf(PC, "\r\nThis  mission is executed\r\n\t-> 0x%d\r\n", command_id);
      return;
   }
   
   add_executed_mission(command_id);
   
   // execution mission
   fprintf(PC, "\r\n____________________________________\r\n________Start execute_mission________\r\n\r\n");
   fprintf(PC, "Command ID: %X\r\n\r\n", command_id);
   switch(command_id)
   {
      case ID_SWEEP:
         mode_measure(parameter);
         break;
      case ID_FLASH_ERASE_ALL:
         break;
      default:
         fprintf(PC, "\t\t-> Invalid CMD ID!\r\n");
   }
   fprintf(PC, "\r\n_________End execute_mission_________\r\n____________________________________\r\n");
}
