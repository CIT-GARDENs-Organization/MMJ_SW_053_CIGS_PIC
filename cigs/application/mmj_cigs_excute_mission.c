#include "../lib/tool/smf_queue.h"                   // ツールライブラリ
#include "../hal/mmj_cigs_config.h"             // システム設定
#include "../lib/communication/value_status.h"    // ステータス定義
#include "mmj_cigs_excute_mission.h"                  // 同じフォルダのヘッダー
#include "mmj_cigs_mode_mission.h"                    // 同じフォルダのヘッダー
#include "mmj_cigs_mode_flash.h"                      // 同じフォルダのヘッダー

#include "../domain/mmj_cigs_smf.h"             // ストレージ機能
#include "../lib/communication/typedef_content.h"    // 通信ライブラリ
#include "../lib/communication/mission_tools.h"      // ミッションツール
#include "../domain/mmj_cigs_iv.h"          // 測定機能
#include "../lib/communication/communication_driver.h" // 通信ドライバー


void execute_mission(int8 *content)
{
   // record to executed mission list
   int8 command_id = content[0];
   //unsigned int8 *parameter = &content[1];
      
   // execution mission
   fprintf(PC, "\r\n______________________________\r\n_____Start execute_mission_____\r\n\r\n");
   fprintf(PC, "Command ID: %X\r\n\r\n", command_id);
   switch(command_id)
   {
      case 0x10: // example command
         // mode_dummy(content);
         //mode_iv_meas_adc();
         break;
      case 0x11:
         // mode_test_iv(content);
         break;
      case 0x12:
         // test_adc();
         break;
      case 0x01:
         //mode_measure(content); // check if the flash is connected
         break;
      case 0x13:
         // mode_sweep_port1(content[1]); // Sweep Port 1 with the given step
         break;

      // ___________________ MISF Commands ______________________
      case ID_MISF_ERASE_ALL:
         mode_misf_erase_all(content);
         break;
      case ID_MISF_ERASE_1SECTOR:
         mode_misf_erase_1sector(content);
         break;
      case ID_MISF_ERASE_4kByte_SUBSECTOR:
         mode_misf_erase_4kbyte_subsector(content);
         break;
      case ID_MISF_ERASE_64kByte_SUBSECTOR:
         mode_misf_erase_64kbyte_subsector(content);
         break;
      case ID_MISF_WRITE_DEMO:
         mode_misf_write_demo(content);
         break;
      case ID_MISF_WRITE_4kByte_SUBSECTOR:
         mode_misf_write_4kbyte_subsector(content);
         break;
      case ID_MISF_READ:
         mode_misf_read(content);
         break;
      case ID_MISF_READ_ADDRESS:
         mode_misf_read_address(content);
         break;
      case ID_MISF_ERASE_AND_RESET:
         mode_misf_erase_and_reset(content);
         break;

      // ___________________ SMF Commands ______________________
      case ID_SMF_COPY:
         mode_smf_copy(content);
         break;
      case ID_SMF_READ:
         mode_smf_read(content);
         break;
      case ID_SMF_ERASE:
         mode_smf_erase(content);
         break;
      case ID_SMF_COPY_FORCE:
         mode_smf_address_reset(content);
         break;
      case ID_SMF_READ_FORCE:
         mode_smf_read_force(content);
         break;
      case ID_SMF_ERASE_FORCE:
         mode_smf_erase_force(content);
         break;
      case ID_SMF_RESET:
         smf_data_table_init();
         break;
         
         
      // ________________MEAS________________________________
      case ID_MEAS_IV:
         mode_meas_iv(content);
         break;
      case ID_MEAS_DEBUG:
         mode_meas_iv_debug(content);
         break;;
      case ID_MEAS_ENV:
         mode_meas_env(content);
         break;
      case ID_MEAS_ENV_DEBUG:
         mode_meas_env_debug(content);
         break;
     
     case 0xA4:
          sweep_with_print();
          break;
     
      default:
         fprintf(PC, "\t\t-> Invalid CMD ID!\r\n");
         break;
   }
   fprintf(PC, "\r\n______End execute_mission______\r\n_______________________________\r\n");
}



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



void handle_uplink_command(Command *command) {
   fprintf(PC, "\t-> Uplink command\r\n");
   fprintf(PC, "\t   Transmit Acknolegde\r\n");
   transmit_ack();

   status[0] = EXECUTING_MISSION;
   execute_mission(command->content);
   if (is_empty_flash_queue())
      status[0] = FINISHED;
   else
      status[0] = SMF_USE_REQ;

   //return TRUE; // ここでTRUEを返すことで、実行後に何か処理をすることができる
   
}


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


void handle_smf_available(Command *command)
{
    fprintf(PC, "\t-> SMF available check\r\n");
    fprintf(PC, "\t   Transmit Acknowledgement\r\n");
    transmit_ack();

    if (command->content[0] != ALLOW) {
        fprintf(PC, "\t\t-> denied\r\n");
        return;
    }

    fprintf(PC, "\t\t-> allowed\r\n");
    status[0] = COPYING;

    int8 processed_count = 0;

    while (!is_empty_flash_queue()) {
        FlashOperationStruct *smf_data = dequeue_flash_operation();
        /*
        if (smf_data == NULL) {
            fprintf(PC, "\t\t-> dequeue NULL (break)\r\n");
            break;
        }
         */
        fprintf(PC, "\t\t-> Dequeued: func=%u mission=%u size=%ld addr=%ld\r\n",
                (unsigned int)smf_data->func_type,
                (unsigned int)smf_data->mission_id,
                smf_data->misf_size,
                smf_data->misf_start_addr);

        switch (smf_data->func_type) {
            case ENUM_SMF_WRITE:
                smf_write(smf_data);
                break;
            case ENUM_SMF_READ:
                smf_read(smf_data);
                break;
            case ENUM_SMF_ERASE:
                smf_erase(smf_data);
                break;
            default:
                fprintf(PC, "\t\t   Unknown func_type=%u\r\n",
                        (unsigned int)smf_data->func_type);
                break;
        }
        processed_count++;
    }

    fprintf(PC, "\t\t-> Completed %u operations\r\n",
            (unsigned int)processed_count);

    status[0] = FINISHED;
}
// End of file
