#include "mmj_cigs_mode_mission.h"                    // 同じフォルダのヘッダー
#include "../domain/mmj_cigs_iv.h"          // 測定機能
#include "../lib/tool/smf_queue.h"                   // ツールライブラリ
#include "../lib/device/mt25q.h"                     // デバイスライブラリ
#include "../lib/device/ad7490.h"                    // ADCライブラリ
#include "../domain/mmj_cigs_flash.h"             // ストレージ機能
#include "../domain/mmj_cigs_piclog.h"            // ログ機能


void mode_meas_iv(unsigned int8 *uplinkcmd_ptr)
{
   fprintf(PC, "Start MODE MEAS IV\r\n");
   MEAS_IV_CMD cmd = make_meas_iv_cmd(uplinkcmd_ptr); // Create the measurement command structure
   fprintf(PC, "\tSleepTime:        %04LX\r\n", cmd.sleep_time);
   fprintf(PC, "\tCurrentThreshold: %04LX\r\n", cmd.curr_threshold);
   fprintf(PC, "\tPDThreshold:      %04LX\r\n", cmd.pd_threshold);
   fprintf(PC, "\tCurrentLimit:     %04LX\r\n", cmd.curr_limit);
   fprintf(PC, "\tMeasurementTime:  %04LX\r\n", cmd.meas_time);

   piclog_make(cmd.id, PICLOG_PARAM_START); // Log the start of the command execution

   unsigned int32 start_time = get_current_sec();
   unsigned int32 current_sec = 0;
   
   sweep(cmd.curr_threshold, cmd.curr_limit, cmd.pd_threshold); // Perform the sweep with thresholds
   delay_ms(cmd.sleep_time);
   /*
   while(get_current_sec() - start_time < cmd.meas_time)
   {
      sweep(cmd.curr_threshold, cmd.curr_limit, cmd.pd_threshold); // Perform the sweep with thresholds
      delay_ms(cmd.sleep_time);
   }
   */
   piclog_make(cmd.id, PICLOG_PARAM_END); // Log the end of the command execution



   FlashOperationStruct data = {0};
   data.func_type = ENUM_SMF_WRITE;
   data.mission_id = CIGS_IV1_DATA; // ID_CIGS_MEASURE_DATA; // コピーする目的のデータ種別
   data.write_mode = SMF_WRITE_CIRCULAR;
   data.source_type = SOURCE_MISF_UNCOPIED;


   fprintf(PC, "Enqueue Flash Operation\r\n");
   fprintf(PC, "Mission ID:   %02X\r\n", data.mission_id);
   fprintf(PC, "Function Type:%02X\r\n", data.func_type);
   fprintf(PC, "Write Mode:   %02X\r\n", data.write_mode);
   fprintf(PC, "Source Type:  %02X\r\n", data.source_type);
   fprintf(PC, "Start Address:%04X\r\n", data.misf_start_addr);
   fprintf(PC, "Size:         %04X\r\n", misf_counter_table[CIGS_IV1_DATA].uncopied_counter);

   enqueue_flash_operation(&data); // SMFへのデータコピーを実行する

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

   unsigned int32 start_time = get_current_time_10ms();
   unsigned int32 current_time = 0;
   test_sweep(cmd.curr_threshold, cmd.curr_limit);

   piclog_make(cmd.id, PICLOG_PARAM_END);
   fprintf(PC, "[IVDBG] End MODE IV DEBUG\r\n");
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

void mode_meas_env(unsigned int8 *uplinkcmd_ptr)
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

       delay_ms(1);  // 過負荷防止用
   }
}

void mode_meas_env_debug(unsigned int8 *uplinkcmd_ptr)
{
   fprintf(PC, "Start MODE MEAS ENV DEBUG\r\n");
   // Add debug-specific implementation here
   fprintf(PC, "End MODE MEAS ENV DEBUG\r\n");
}
