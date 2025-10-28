#include "cigs_mode_mission.h"                    // 同じフォルダのヘッダー
#include "../domain/cigs_iv.h"          // 測定機能
#include "../lib/tool/smf_queue.h"                   // ツールライブラリ
#include "../lib/device/mt25q.h"                     // デバイスライブラリ
#include "../lib/device/ad7490.h"                    // ADCライブラリ
#include "../domain/cigs_flash.h"             // ストレージ機能
#include "../domain/cigs_piclog.h"            // ログ機能


void mode_meas_iv_1sweep(unsigned int8 *uplinkcmd_ptr)
{
   fprintf(PC, "Start MODE MEAS IV 1 SWEEP\r\n");

   mode_misf_erase_and_reset(uplinkcmd_ptr);

   meas_iv_param_t cmd = make_meas_iv_cmd(uplinkcmd_ptr); // Create the measurement command structure
   cmd.sweep_limit = 0x0030;
   sweep(cmd.log_threshold, cmd.sweep_limit, cmd.pd_threshold);

   FlashOperationStruct data = {0};
   data.func_type = ENUM_SMF_WRITE;
   data.mission_id = CIGS_IV1_DATA; // ID_CIGS_MEASURE_DATA; // コピーする目的のデータ種別
   data.write_mode = SMF_WRITE_CIRCULAR;
   data.source_type = SOURCE_MISF_UNCOPIED;
   fprintf(PC, "\r\nADD SMF QUEUE\r\n");
   fprintf(PC, "\tMission ID:   %02X\r\n", data.mission_id);
   fprintf(PC, "\tFunction Type:%02X\r\n", data.func_type);
   fprintf(PC, "\tWrite Mode:   %02X\r\n", data.write_mode);
   fprintf(PC, "\tSource Type:  %02X\r\n", data.source_type);
   fprintf(PC, "\tStart Address:%04X\r\n", data.misf_start_addr);
   fprintf(PC, "\tSize:         %04X\r\n", misf_counter_table[CIGS_IV1_DATA].uncopied_counter);
   enqueue_flash_operation(&data); // SMFへのデータコピーを実行する


   data.func_type = ENUM_SMF_WRITE;
   data.mission_id = CIGS_IV2_DATA; // ID_CIGS_MEASURE_DATA; // コピーする目的のデータ種別
   data.write_mode = SMF_WRITE_CIRCULAR;
   data.source_type = SOURCE_MISF_UNCOPIED;
   fprintf(PC, "\r\nADD SMF QUEUE\r\n");
   fprintf(PC, "\tMission ID:   %02X\r\n", data.mission_id);
   fprintf(PC, "\tFunction Type:%02X\r\n", data.func_type);
   fprintf(PC, "\tWrite Mode:   %02X\r\n", data.write_mode);
   fprintf(PC, "\tSource Type:  %02X\r\n", data.source_type);
   fprintf(PC, "\tStart Address:%04X\r\n", data.misf_start_addr);
   fprintf(PC, "\tSize:         %04X\r\n", misf_counter_table[CIGS_IV2_DATA].uncopied_counter);
   enqueue_flash_operation(&data); // SMFへのデータコピーを実行する

   misf_update_address_area();
   print_misf_address_table();
}

void mode_meas_iv_port1(unsigned int8 *uplinkcmd_ptr)
{
   fprintf(PC, "Start MODE MEAS IV PORT1\r\n");

   mode_misf_erase_and_reset(uplinkcmd_ptr);

   meas_iv_param_t cmd = make_meas_iv_cmd(uplinkcmd_ptr); // Create the measurement command structure
   sweep_port1(cmd.sweep_limit);

   FlashOperationStruct data = {0};
   data.func_type = ENUM_SMF_WRITE;
   data.mission_id = CIGS_IV1_DATA; // ID_CIGS_MEASURE_DATA; // コピーする目的のデータ種別
   data.write_mode = SMF_WRITE_CIRCULAR;
   data.source_type = SOURCE_MISF_UNCOPIED;
   fprintf(PC, "\r\nADD SMF QUEUE\r\n");
   fprintf(PC, "\tMission ID:   %02X\r\n", data.mission_id);
   fprintf(PC, "\tFunction Type:%02X\r\n", data.func_type);
   fprintf(PC, "\tWrite Mode:   %02X\r\n", data.write_mode);
   fprintf(PC, "\tSource Type:  %02X\r\n", data.source_type);
   fprintf(PC, "\tStart Address:%04X\r\n", data.misf_start_addr);
   fprintf(PC, "\tSize:         %04X\r\n", misf_counter_table[CIGS_IV1_DATA].uncopied_counter);
   enqueue_flash_operation(&data); // SMFへのデータコピーを実行する

   misf_update_address_area();
}

void mode_meas_iv_port2(unsigned int8 *uplinkcmd_ptr)
{
   fprintf(PC, "Start MODE MEAS IV PORT2\r\n");

   mode_misf_erase_and_reset(uplinkcmd_ptr);
   meas_iv_param_t cmd = make_meas_iv_cmd(uplinkcmd_ptr); // Create the measurement command structure
   sweep_port1(cmd.sweep_limit);

   FlashOperationStruct data = {0};
   data.func_type = ENUM_SMF_WRITE;
   data.mission_id = CIGS_IV2_DATA; // ID_CIGS_MEASURE_DATA; // コピーする目的のデータ種別
   data.write_mode = SMF_WRITE_CIRCULAR;
   data.source_type = SOURCE_MISF_UNCOPIED;
   fprintf(PC, "\r\nADD SMF QUEUE\r\n");
   fprintf(PC, "\tMission ID:   %02X\r\n", data.mission_id);
   fprintf(PC, "\tFunction Type:%02X\r\n", data.func_type);
   fprintf(PC, "\tWrite Mode:   %02X\r\n", data.write_mode);
   fprintf(PC, "\tSource Type:  %02X\r\n", data.source_type);
   fprintf(PC, "\tStart Address:%04X\r\n", data.misf_start_addr);
   fprintf(PC, "\tSize:         %04X\r\n", misf_counter_table[CIGS_IV2_DATA].uncopied_counter);
   enqueue_flash_operation(&data); // SMFへのデータコピーを実行する

   misf_update_address_area();







}


void mode_meas_iv(unsigned int8 *uplinkcmd_ptr)
{
   fprintf(PC, "Start MODE MEAS IV\r\n");
   meas_iv_param_t cmd = make_meas_iv_cmd(uplinkcmd_ptr); // Create the measurement command structure
   print_meas_iv_cmd(&cmd); // Print the command parameters for debugging
   piclog_make(cmd.id, PICLOG_PARAM_START); // Log the start of the command execution

   unsigned int32 start_time = get_current_sec();
   unsigned int32 current_sec = 0;
   unsigned int16 time_ms = 0;
   unsigned int8 counter = 0;
   do
   {
      sweep(cmd.log_threshold, cmd.sweep_limit, cmd.pd_threshold); 
      if (cmd.interval > 0){
         delay_ms(cmd.interval);
      }
      check_and_respond_to_boss(); // Check for boss commands during the wait period
   } while (get_current_sec() - start_time < cmd.meas_time);
   misf_update_address_area();
   piclog_make(cmd.id, PICLOG_PARAM_END); // Log the end of the command execution



   FlashOperationStruct data = {0};
   data.func_type = ENUM_SMF_WRITE;
   data.mission_id = CIGS_IV1_DATA; // ID_CIGS_MEASURE_DATA; // コピーする目的のデータ種別
   data.write_mode = SMF_WRITE_CIRCULAR;
   data.source_type = SOURCE_MISF_UNCOPIED;
   fprintf(PC, "\r\nADD SMF QUEUE\r\n");
   fprintf(PC, "\tMission ID:   %02X\r\n", data.mission_id);
   fprintf(PC, "\tFunction Type:%02X\r\n", data.func_type);
   fprintf(PC, "\tWrite Mode:   %02X\r\n", data.write_mode);
   fprintf(PC, "\tSource Type:  %02X\r\n", data.source_type);
   fprintf(PC, "\tStart Address:%04X\r\n", data.misf_start_addr);
   fprintf(PC, "\tSize:         %04X\r\n", misf_counter_table[CIGS_IV1_DATA].uncopied_counter);
   enqueue_flash_operation(&data); // SMFへのデータコピーを実行する


   data.func_type = ENUM_SMF_WRITE;
   data.mission_id = CIGS_IV2_DATA; // ID_CIGS_MEASURE_DATA; // コピーする目的のデータ種別
   data.write_mode = SMF_WRITE_CIRCULAR;
   data.source_type = SOURCE_MISF_UNCOPIED;
   fprintf(PC, "\r\nADD SMF QUEUE\r\n");
   fprintf(PC, "\tMission ID:   %02X\r\n", data.mission_id);
   fprintf(PC, "\tFunction Type:%02X\r\n", data.func_type);
   fprintf(PC, "\tWrite Mode:   %02X\r\n", data.write_mode);
   fprintf(PC, "\tSource Type:  %02X\r\n", data.source_type);
   fprintf(PC, "\tStart Address:%04X\r\n", data.misf_start_addr);
   fprintf(PC, "\tSize:         %04X\r\n", misf_counter_table[CIGS_IV2_DATA].uncopied_counter);
   enqueue_flash_operation(&data); // SMFへのデータコピーを実行する


   FlashOperationStruct piclog = {0};
   piclog.func_type = ENUM_SMF_WRITE;
   piclog.mission_id = FLASH_ID_PICLOG; // ID_CIGS_MEASURE_DATA; // コピーする目的のデータ種別
   piclog.write_mode = SMF_WRITE_CIRCULAR;
   piclog.source_type = SOURCE_MISF_UNCOPIED;

   fprintf(PC, "\r\nADD SMF QUEUE\r\n");
   fprintf(PC, "\tMission ID:   %02X\r\n", piclog.mission_id);
   fprintf(PC, "\tFunction Type:%02X\r\n", piclog.func_type);
   fprintf(PC, "\tWrite Mode:   %02X\r\n", piclog.write_mode);
   fprintf(PC, "\tSource Type:  %02X\r\n", piclog.source_type);
   fprintf(PC, "\tStart Address:%04X\r\n", piclog.misf_start_addr);
   fprintf(PC, "\tSize:         %04X\r\n", misf_counter_table[FLASH_ID_PICLOG].uncopied_counter);
   enqueue_flash_operation(&piclog); // SMFへのデータコピーを実行する
   fprintf(PC, "End MODE MEAS IV mission\r\n");
}

void mode_meas_iv_debug(unsigned int8 *uplinkcmd_ptr)
{
   fprintf(PC, "Start MODE IV DEBUG\r\n");

   MEAS_IV_CMD cmd = make_meas_iv_cmd(uplinkcmd_ptr); // Create the measurement command structure
   
   fprintf(PC, "\tID: %02X\r\n", cmd.id);
   fprintf(PC, "\tSleepTime:        0x%04X\r\n", cmd.sleep_time);
   fprintf(PC, "\tLogCurrThreshold: 0x%04X\r\n", cmd.curr_threshold);
   fprintf(PC, "\tPDThreshold:      0x%04X\r\n", cmd.pd_threshold);
   fprintf(PC, "\tCurrentLimit:     0x%04X\r\n", cmd.curr_limit);
   fprintf(PC, "\tMeasurementTime:  0x%04X\r\n", cmd.meas_time);

   piclog_make(cmd.id, PICLOG_PARAM_START); // Log start

   unsigned int32 start_time = get_current_sec();
   unsigned int32 current_time = 0;
   test_sweep(cmd.curr_threshold, cmd.curr_limit);

   piclog_make(cmd.id, PICLOG_PARAM_END);
   fprintf(PC, "[IVDBG] End MODE IV DEBUG\r\n");
}




void mode_meas_env(unsigned int8 *uplinkcmd_ptr)
{
   unsigned int16 interval = 100;   // 測定間隔 [秒]
   unsigned int16 end_time = 0xFFFF;

   unsigned int32 current_time = get_current_sec();
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
       current_time = get_current_sec();

       // intervalごとに測定
       if ((current_time - last_time) >= interval) {
           last_time = current_time;
         /*
           // 次の測定データ(6バイト)で64バイト境界を超えるか確認
           if (environment_data_ptr->reserve_counter1 + 6 > PACKET_SIZE) {
               unsigned int8 bankdata[4] = {0};
               write_address = MISF_CIGS_ENVIRO_START + environment_data_ptr->used_counter;
               write_data_bytes(mis_fm, write_address, bankdata, 4);
               environment_data_ptr->used_counter   += 4;
               environment_data_ptr->uncopied_counter += 4;
               environment_data_ptr->reserve_counter1 = 0;
               misf_update_address_area();
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
         */
       delay_ms(1);  // 過負荷防止用
   }
}
}
// make cmd structure from uplink command
meas_iv_param_t make_meas_iv_cmd(int8 *uplinkcmd_ptr)
{
   meas_iv_param_t cmd;
   cmd.id = uplinkcmd_ptr[0];
   cmd.interval = ((unsigned int16)uplinkcmd_ptr[1] << 8) | ((unsigned int16)uplinkcmd_ptr[2]);
   cmd.log_threshold = (unsigned int16)uplinkcmd_ptr[3]<< 4;
   cmd.sweep_limit = (unsigned int16)uplinkcmd_ptr[4]<< 4;
   cmd.pd_threshold = (unsigned int16)uplinkcmd_ptr[5]<< 4;
   cmd.meas_time = (unsigned int16)uplinkcmd_ptr[6] *60; // 分 -> 秒
   return cmd;
}

void print_meas_iv_cmd(meas_iv_param_t *cmd_ptr)
{
   // Calc
   unsigned int16 pd_value = calc_pd_value(cmd_ptr->pd_threshold);
   int16 sweep_limit = calc_curr_value(cmd_ptr->sweep_limit);
   int16 log_threshold = calc_curr_value(cmd_ptr->log_threshold);

   // Print
   fprintf(PC, "MEAS IV Command Parameters:\r\n");
   fprintf(PC, "\tID: %02X\r\n", cmd_ptr->id);
   fprintf(PC, "\tSweepInterval   :%lu[ms]\r\n", cmd_ptr->interval);
   fprintf(PC, "\tPD Threshold    :%lu[W/m2]\r\n", pd_value);
   fprintf(PC, "\tSweepLimit      :%lu[mA]\r\n", sweep_limit);
   fprintf(PC, "\tLogThreshold    :%lu[mA]\r\n", log_threshold);
   fprintf(PC, "\tMeasurementTime :%lu[s]\r\n\r\n", cmd_ptr->meas_time);
}

void mode_meas_pd(unsigned int8 *uplinkcmd_ptr)
{
   fprintf(PC, "Start MODE MEAS PD\r\n");

   // uplinkcmd_ptr から interval と測定時間を取り出す
   unsigned int16 interval_ms = ((unsigned int16)uplinkcmd_ptr[1] << 8) | uplinkcmd_ptr[2]; // ms
   unsigned int16 meas_time_s = (unsigned int16)uplinkcmd_ptr[6] * 60; // 秒

   unsigned int32 start_time_ms   = get_current_msec();
   unsigned int32 current_time_ms = 0;
   unsigned int32 meas_time_ms    = (unsigned int32)meas_time_s * 1000; // ms に変換

   while (current_time_ms - start_time_ms < meas_time_ms) {

      current_time_ms = get_current_msec();

      // interval ごとに実行
      static unsigned int32 last_meas_time = 0;
      if (current_time_ms - last_meas_time >= interval_ms) {
         last_meas_time = current_time_ms;

         // PD センサー読み取り
         unsigned int16 pd = ad7490_read(ADC_PD);

         // デバッグ出力
         // fprintf(PC, "Time:%lu ms, PD:%u\r\n", current_time_ms - start_time_ms, pd);
      }
   }

   fprintf(PC, "End MODE MEAS PD\r\n");
}
