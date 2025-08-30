#include "mmj_cigs_mode_mission.h"                    // 同じフォルダのヘッダー
#include "../domain/mmj_cigs_iv.h"          // 測定機能
#include "../lib/tool/smf_queue.h"                   // ツールライブラリ
#include "../lib/device/mt25q.h"                     // デバイスライブラリ
#include "../lib/device/ad7490.h"                    // ADCライブラリ
#include "../domain/mmj_cigs_flash.h"             // ストレージ機能
#include "../domain/mmj_cigs_piclog.h"            // ログ機能

// ___________________ Mode Functions ______________________
void mode_dummy(unsigned int8 uplinkcmd[])
{
   fprintf(PC, "Start MODE DUMMY\r\n");

   DUMMY_CMD dummy_cmd;
   dummy_cmd.id = uplinkcmd[0]; // Get the command ID from the uplink command
   dummy_cmd.param1 = 
      ((unsigned int32)uplinkcmd[1] << 24) |
      ((unsigned int32)uplinkcmd[2] << 16) |
      ((unsigned int32)uplinkcmd[3] << 8)  |
      ((unsigned int32)uplinkcmd[4]);
   dummy_cmd.param2 = 
      ((unsigned int16)uplinkcmd[5] << 8) |
      ((unsigned int16)uplinkcmd[6]);
   fprintf(PC, "\tMODE     : %02X\r\n", dummy_cmd.id);
   fprintf(PC, "\tParam1   : 0x%08LX\r\n", dummy_cmd.param1);
   fprintf(PC, "\tParam2   : 0x%04LX\r\n", dummy_cmd.param2);

   piclog_make(dummy_cmd.id, PICLOG_PARAM_START); // Log the command execution

   // This is a dummy function for testing purposes
   // You can add your own code here
   piclog_make(dummy_cmd.id, PICLOG_PARAM_END); // Log the end of the command execution
   fprintf(PC, "End MODE DUMMY\r\n");
}


void mode_test_iv(unsigned int8 *uplinkcmd)
{
   fprintf(PC, "Start MODE MEAS IV\r\n");
   MEAS_IV_CMD cmd = make_meas_iv_cmd(uplinkcmd); // Create the measurement command structure
   fprintf(PC, "\tID: %02X\r\n", cmd.id);
   fprintf(PC, "\tSleep Time: %04LX ms\r\n", cmd.sleep_time);
   fprintf(PC, "\tCurrent Threshold: %04LX mA\r\n", cmd.curr_threshold);
   fprintf(PC, "\tPD Threshold: %04LX mA\r\n", cmd.pd_threshold);
   fprintf(PC, "\tCurrent Limit: %04LX mA\r\n", cmd.curr_limit);
   fprintf(PC, "\tMeasurement Time: %04LX s\r\n", cmd.meas_time);
   output_high(CONNECT_CIGS1);
   output_low(EN_NPWR); // Enable NPWR
   unsigned int16 readdata;

   test_sweep(cmd.curr_threshold,cmd.curr_limit); // Call the sweep function with the measurement step

   fprintf(PC, "End MODE TEST IV\r\n");
   output_low(CONNECT_CIGS1);
   output_high(EN_NPWR); // Disable NPWR
}



void mode_meas_iv(unsigned int8 *uplinkcmd)
{
   fprintf(PC, "Start MODE MEAS IV\r\n");
   MEAS_IV_CMD cmd = make_meas_iv_cmd(uplinkcmd); // Create the measurement command structure
   fprintf(PC, "\tID: %02X\r\n", cmd.id);
   fprintf(PC, "\tSleep Time: %04LX ms\r\n", cmd.sleep_time);
   fprintf(PC, "\tCurrent Threshold: %04LX mA\r\n", cmd.curr_threshold);
   fprintf(PC, "\tPD Threshold: %04LX mA\r\n", cmd.pd_threshold);
   fprintf(PC, "\tCurrent Limit: %04LX mA\r\n", cmd.curr_limit);
   fprintf(PC, "\tMeasurement Time: %04LX s\r\n", cmd.meas_time);
//!   fprintf(PC, "\tIs Finished: %u\r\n", cmd.is_finished);

   piclog_make(cmd.id, PICLOG_PARAM_START); // Log the start of the command execution

   unsigned int16 start_time = get_current_sec();
   unsigned int16 current_sec = 0;
   while(get_current_sec() - start_time < cmd.meas_time)
   {
      current_sec = get_current_sec();
      if (current_sec - start_time >= cmd.meas_time) {
          break;
      }
      //sweep(cmd.curr_threshold, cmd.pd_threshold, cmd.curr_limit); // Perform the sweep with thresholds
      delay_ms(cmd.sleep_time);
   }
   piclog_make(cmd.id, PICLOG_PARAM_END); // Log the end of the command execution

   FlashOperationStruct data = {0};
   data.func_type = ENUM_SMF_WRITE;
   data.mission_id = CIGS_IV1_DATA; // ID_CIGS_MEASURE_DATA; // コピーする目的のデータ種別
   data.write_mode = SMF_WRITE_CIRCULAR;
   data.source_type = SOURCE_MISF_UNCOPIED;
   // data.src = ADDRESS_MISF_MEASUREMENT_START + cigs_counters.counters.misf_meas_use_counter - cigs_counters.counters.misf_meas_uncopyed_counter; // コピー元のMIS_FMのアドレス
   // data.size = 0; // コピーするデータのサイズ

   fprintf(PC, "Enqueue Flash Operation\r\n");
   fprintf(PC, "Mission ID:   %02X\r\n", data.mission_id);
   fprintf(PC, "Function Type:%02X\r\n", data.func_type);
   fprintf(PC, "Write Mode:   %02X\r\n", data.write_mode);
   fprintf(PC, "Source Type:  %02X\r\n", data.source_type);
   fprintf(PC, "Start Address:%04X\r\n", data.misf_start_addr);
   fprintf(PC, "Size:         %04X\r\n", data.misf_size);

   enqueue_flash_operation(&data); // SMFへのデータコピーを実行する

   fprintf(PC, "End MODE MEAS IV mission\r\n");
}

void mode_iv_test(unsigned int8 *uplinkcmd)
{
   fprintf(PC, "Start MODE IV TEST \r\n");
   MEAS_IV_CMD cmd = make_meas_iv_cmd(uplinkcmd); // Create the measurement command structure
   fprintf(PC, "\tID: %02X\r\n", cmd.id);
   fprintf(PC, "\tSleep Time: %04LX ms\r\n", cmd.sleep_time);
   fprintf(PC, "\tCurrent Threshold: %04LX mA\r\n", cmd.curr_threshold);
   fprintf(PC, "\tPD Threshold: %04LX mA\r\n", cmd.pd_threshold);
   fprintf(PC, "\tCurrent Limit: %04LX mA\r\n", cmd.curr_limit);
   fprintf(PC, "\tMeasurement Time: %04LX s\r\n", cmd.meas_time);
//!   fprintf(PC, "\tIs Finished: %u\r\n", cmd.is_finished);

   piclog_make(cmd.id, PICLOG_PARAM_START); // Log the start of the command execution

   unsigned int16 start_time = get_current_sec();
   unsigned int16 current_sec = 0;
   while(get_current_sec() - start_time < cmd.meas_time)
   {
      current_sec = get_current_sec();
      if (current_sec - start_time >= cmd.meas_time) {
          break;
      }
      test_sweep(cmd.curr_threshold, cmd.curr_limit); // Perform the sweep with thresholds
      delay_ms(cmd.sleep_time);
   }
   piclog_make(cmd.id, PICLOG_PARAM_END); // Log the end of the command execution




}




MEAS_IV_CMD make_meas_iv_cmd(unsigned int8 *uplinkcmd[])
{
   MEAS_IV_CMD cmd;
   cmd.id = uplinkcmd[0];
   cmd.sleep_time = ((unsigned int16)uplinkcmd[1] << 8) | ((unsigned int16)uplinkcmd[2]);
   cmd.curr_threshold = (unsigned int16)uplinkcmd[3]<< 4;
   cmd.pd_threshold = (unsigned int16)uplinkcmd[4]<< 4;
   cmd.curr_limit = (unsigned int16)uplinkcmd[5]<< 4;
   cmd.meas_time = ((unsigned int16)uplinkcmd[6] << 8) | ((unsigned int16)uplinkcmd[7]);
//!  cmd.is_finished = uplinkcmd[8];
   return cmd;
}

void mode_meas_enviro()
{
   unsigned int16 interval = 100;   // 測定間隔 [秒]
   unsigned int16 end_time = 0xFFFF;

   unsigned int32 current_time = get_current_time_10ms();
   unsigned int32 last_time = current_time - interval;  // 最初にすぐ測定するため

   // loging data
   unsigned int8 logdata[6];
   unsigned int16 temp_top;
   unsigned int16 temp_bot;
   unsigned int16 temp_mis7;
   unsigned int16 PD;

   // Flash Write 
   unsigned int32 write_address;

   fprintf(PC, "temp_top, temp_bot, temp_mis7, pd\r\n");

   while (true) {
       current_time = get_current_time_10ms();

       // intervalごとに測定
       if ((current_time - last_time) >= interval) {
           last_time = current_time;

           // ★ 次の測定データ(6バイト)で64バイト境界を超えるか確認
           if (environment_data_ptr->reserve_counter1 + 6 > PACKET_SIZE) {
               unsigned int8 bankdata[4] = {0};
               write_address = MISF_CIGS_ENVIRO_START + environment_data_ptr->used_counter;
               write_data_bytes(mis_fm, write_address, bankdata, 4);
               environment_data_ptr->used_counter   += 4;
               environment_data_ptr->uncopied_counter += 4;
               environment_data_ptr->reserve_counter1 = 0;
               write_misf_address_area();
           }

           // 環境データ読み取り
           temp_top  = ad7490_read(ADC_TEMP_PY_TOP);
           temp_bot  = ad7490_read(ADC_TEMP_PY_BOT);
           temp_mis7 = ad7490_read(ADC_TEMP_MIS7);
           PD        = ad7490_read(ADC_PD);

           logdata[0] = (temp_top >> 4) & 0xFF;
           logdata[1] = ((temp_top & 0x0F) << 4) | ((temp_bot >> 8) & 0x0F);
           logdata[2] = temp_bot & 0xFF;
           logdata[3] = (temp_mis7 >> 4) & 0xFF;
           logdata[4] = temp_mis7 & 0xFF;
           logdata[5] = PD & 0xFF;

           // 書き込み
           write_address = MISF_CIGS_ENVIRO_START + environment_data_ptr->used_counter;
           write_data_bytes(mis_fm, write_address, logdata, 6);
           environment_data_ptr->used_counter   += 6;
           environment_data_ptr->uncopied_counter += 6;
           environment_data_ptr->reserve_counter1 += 6;
           fprintf(PC, "0x%04LX, 0x%04LX, 0x%04LX, 0x%04LX\r\n", temp_top, temp_bot, temp_mis7, PD);
       }

       delay_ms(100);  // 過負荷防止用
   }
}

