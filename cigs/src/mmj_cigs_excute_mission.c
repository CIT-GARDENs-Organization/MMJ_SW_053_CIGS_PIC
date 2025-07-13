#include "../mmj_cigs_excute_mission.h"
#include "../mmj_cigs_mode.h"
#include "../mmj_cigs_config.h"
#include "../../lib/typedef_content.h"
#include "../../lib/smf_queue.h"
#include "../../lib/mt25q.h"
#include "../../lib/mission_tools.h"
#include "../mmj_cigs_piclog.h"

#separate
void execute_mission(unsigned int8 *content)
{
   // record to executed mission list
   unsigned int8 command_id = content[0];
   //unsigned int8 *parameter = &content[1];
      
   // execution mission
   fprintf(PC, "\r\n______________________________\r\n_____Start execute_mission_____\r\n\r\n");
   fprintf(PC, "Command ID: %X\r\n\r\n", command_id);
   switch(command_id)
   {
      case ID_DUMMY: // example command
         mode_dummy(content);
         //mode_iv_meas_adc();
         break;
      case 0x10:
         mode_test_iv(content);
         break;
      case 0x01:
         //example_01(parameter);
         break;
      case 0x02:
         //example_02(parameter);
         break;
      case ID_FLASH_ERASE_ALL:
         mode_flash_erase_all(content);
         break;
      /*
      case ID_FLASH_ERASE_1SECTOR:
         mode_flash_erase_1sector(content);
         break;
      case ID_FLASH_ERASE_4kByte_SUBSECTOR:
         mode_flash_erase_4kbyte_subsector(content);
         break;
      case ID_FALSH_WRITE_DEMO:
         mode_flash_write_demo(content);
         break;
      case ID_FLASH_4kByte_SUBSECTOR:
         mode_flash_write_4kbyte_subsecotr(content);
         break;
      */
      case ID_FLASH_READ:
         mode_flash_read(content);
         break;
      /*
      case ID_FLASH_READ_ADDRESS:
         mode_flash_read_address(content);
         break;
      case ID_FLASH_SMF_COPY:
         mode_flash_smf_copy(content);
         break;
      */
      case ID_FLASH_ADDRESS_RESET:
         mode_flash_address_reset(content);
         break;
      case 0x71: // example command
         mode_iv_meas_adc();
         break;
      case 0x72: // example command
         mode_iv_meas_adc();
         break;
      case 0x73: // example command
         mode_iv_meas_adc();
         break;

      case 0x70: // example command
         mode_iv_meas_adc();
         break;
      default:
         fprintf(PC, "\t\t-> Invalid CMD ID!\r\n");
   }
   fprintf(PC, "\r\n______End execute_mission______\r\n_______________________________\r\n");
}


#separate
int1 execute_command(Command *command)
{
   fprintf(PC, "Start execute_command\r\n");
   
   switch(command->frame_id)
   {
      case UPLINK_COMMAND:
         handle_uplink_command(command);
         break;
      
      case STATUS_CHECK:
         if (handle_status_check(command)){
            return TRUE; // if status is finished, return TRUE
         }        
         break;
      
      case IS_SMF_AVAILABLE:
         handle_smf_available(command);
         break;
   }
   return FALSE;
}


#Separate
void handle_uplink_command(Command *command) {
   fprintf(PC, "\t-> Uplink command\r\n");
   fprintf(PC, "\t   Transmit Acknolegde\r\n");
   transmit_ack();

   status[0] = EXECUTING_MISSION;
   execute_mission(command->content);
   if (is_empty_smf_data())
      status[0] = FINISHED;
   else
      status[0] = SMF_USE_REQ;

   //return TRUE; // ここでTRUEを返すことで、実行後に何か処理をすることができる
   
}

#Separate
int1 handle_status_check(Command *command) {
   fprintf(PC, "\t-> Status check\r\n");
   fprintf(PC, "\t   Transmit MIS MCU Status\r\n");
   transmit_status();
   if (status[0] == FINISHED)
   {
      fprintf(PC, "finished in status_check\r\n");
      return TRUE;
   }
   else{
      return FALSE;
   }

}

#Separate
void handle_smf_available(Command *command) {
    fprintf(PC, "\t-> SMF available check\r\n");
    fprintf(PC, "\t   Transmit Acknolegde\r\n");
    transmit_ack();
    
    if (command->content[0] == ALLOW) {
        fprintf(PC, "\t\t-> allowd\r\n");
        status[0] = COPYING;
        copy_data();
        status[0] = FINISHED;
    } else {
        fprintf(PC, "\t\t-> denyed\r\n");
    }
}
