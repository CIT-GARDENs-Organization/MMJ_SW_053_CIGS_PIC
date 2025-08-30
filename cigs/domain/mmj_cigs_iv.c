#include "mmj_cigs_iv.h"               // 同じフォルダのヘッダー
#include "../hal/mmj_cigs_config.h"      // システム設定
#include "../device_driver/ad7490_driver.h"   // デバイス定義  
#include "../device_driver/mcp4901_driver.h"
#include "mmj_cigs_flash.h"               // ストレージ機能
#include "../lib/communication/mission_tools.h"   // 通信ツール
#include "../hal/timer.h"

void test_sweep(unsigned int16 curr_threshold,unsigned int16 curr_limit)
{
    fprintf(PC, ".");
   
    // Enable both CIGS ports
    connect_port1();
    connect_port2();

    // Init Port1
    sweep_config_t port1 = {0};
    port1.port_num = 1;
    port1.sweep_step = 0;
    port1.active = 1;

    // Init Port2
    sweep_config_t port2 = {0};
    port2.port_num = 2;
    port2.sweep_step = 0;
    port2.active = 1;

    int16 count = 1;
    
    // Initialize DACs to 0
    mcp4901_1_write(1);
    mcp4901_2_write(1);

    iv_env_t measured_data = create_meas_data();
    while (port1.active || port2.active)
    {
        mcp4901_1_write(count);
        mcp4901_2_write(count);
        delay_us(10); // wait for DAC to stabilize

        if (port1.active) {
            port1.data_buffer[0][count] = ad7490_read(ADC_CIGS1_AMP);
            port1.data_buffer[1][count] = ad7490_read(ADC_CIGS1_CURR);
            port1.sweep_step = count + 1; // Update CIGS1 step counter
        }
        

        if (port2.active) {
            port2.data_buffer[0][count] = ad7490_read(ADC_CIGS2_AMP);
            port2.data_buffer[1][count] = ad7490_read(ADC_CIGS2_CURR);
            // fprintf(PC, "CIGS2 data: %04LX, %04LX\r\n", port2.data_buffer[0][count], port2.data_buffer[1][count]);
            port2.sweep_step = count + 1; // Update CIGS2 step counter
        }

        count++;

        // Check global exit conditions
        if (count >= 255) {
            // fprintf(PC, "Maximum step count reached: %ld\r\n", count);
            break;
        }
        
        // Check CIGS1 specific conditions
        if (port1.active) {
            if (port1.data_buffer[1][count-1] < curr_limit) {
                // fprintf(PC, "CIGS1 current limit reached:");
                port1.active = 0;
                output_low(CONNECT_CIGS1); // Disconnect CIGS1
            }
            else if (port1.data_buffer[1][count-1] < curr_threshold) {
                // fprintf(PC, "CIGS1 current below threshold");
                port1.active = 0;
                output_low(CONNECT_CIGS1); // Disconnect CIGS1
            }
        }
        
        // Check CIGS2 specific conditions
        if (port2.active) {
            if (port2.data_buffer[1][count-1] < curr_limit) {
                // fprintf(PC, "CIGS2 current limit reached: ");
                port2.active = 0;
                output_low(CONNECT_CIGS2); // Disconnect CIGS2
            }
            else if (port2.data_buffer[1][count-1] < curr_threshold) {
                // fprintf(PC, "CIGS2 current below threshold:");
                port2.active = 0;
                output_low(CONNECT_CIGS2); // Disconnect CIGS2
            }
        }
    }

    // Ensure all connections are disabled
    output_low(CONNECT_CIGS1);
    output_low(CONNECT_CIGS2);
    log_meas_data_with_print(&measured_data, &port1); // Log data for CIGS1
    log_meas_data_with_print(&measured_data, &port2); // Log data for CIGS2
}


void sweep(  unsigned int16 curr_threshold, unsigned int16 curr_limit)
{
    fprintf(PC, ".");
   
    // Enable both CIGS ports
    output_high(CONNECT_CIGS1);
    output_high(CONNECT_CIGS2);

    // delay_us(100); // wait for the CIGS to stabilize

    // Init Port1
    sweep_config_t port1 = {0};
    port1.port_num = 1;
    port1.sweep_step = 0;
    port1.active = 1;

    // Init Port2
    sweep_config_t port2 = {0};
    port2.port_num = 2;
    port2.sweep_step = 0;
    port2.active = 1;

    int16 count = 1;
    
    // Initialize DACs to 0
    mcp4901_1_write(1);
    mcp4901_2_write(1);

    iv_env_t measured_data = create_meas_data();
    while (port1.active || port2.active)
    {
        mcp4901_1_write(count);
        mcp4901_2_write(count);
        delay_us(10); // wait for DAC to stabilize

        if (port1.active) {
            port1.data_buffer[0][count] = ad7490_read(ADC_CIGS1_AMP);
            port1.data_buffer[1][count] = ad7490_read(ADC_CIGS1_CURR);
            port1.sweep_step = count + 1; // Update CIGS1 step counter
        }
        

        if (port2.active) {
            port2.data_buffer[0][count] = ad7490_read(ADC_CIGS2_AMP);
            port2.data_buffer[1][count] = ad7490_read(ADC_CIGS2_CURR);
            // fprintf(PC, "CIGS2 data: %04LX, %04LX\r\n", port2.data_buffer[0][count], port2.data_buffer[1][count]);
            port2.sweep_step = count + 1; // Update CIGS2 step counter
        }

        count++;

        // Check global exit conditions
        if (count >= 255) {
            // fprintf(PC, "Maximum step count reached: %ld\r\n", count);
            break;
        }
        
        // Check CIGS1 specific conditions
        if (port1.active) {
            if (port1.data_buffer[1][count-1] < curr_limit) {
                // fprintf(PC, "CIGS1 current limit reached:");
                port1.active = 0;
                output_low(CONNECT_CIGS1); // Disconnect CIGS1
            }
            else if (port1.data_buffer[1][count-1] < curr_threshold) {
                // fprintf(PC, "CIGS1 current below threshold");
                port1.active = 0;
                output_low(CONNECT_CIGS1); // Disconnect CIGS1
            }
        }
        
        // Check CIGS2 specific conditions
        if (port2.active) {
            if (port2.data_buffer[1][count-1] < curr_limit) {
                // fprintf(PC, "CIGS2 current limit reached: ");
                port2.active = 0;
                output_low(CONNECT_CIGS2); // Disconnect CIGS2
            }
            else if (port2.data_buffer[1][count-1] < curr_threshold) {
                // fprintf(PC, "CIGS2 current below threshold:");
                port2.active = 0;
                output_low(CONNECT_CIGS2); // Disconnect CIGS2
            }
        }
    }

    // Ensure all connections are disabled
    output_low(CONNECT_CIGS1);
    output_low(CONNECT_CIGS2);
    log_meas_data(&measured_data, &port1); // Log data for CIGS1
    log_meas_data(&measured_data, &port2); // Log data for CIGS2

    //fprintf(PC, "End SWEEP with threshold\r\n");
}


void log_meas_data(iv_env_t *measured_data_ptr, sweep_config_t *port_data_ptr)
{
    unsigned int8 packetdata[PACKET_SIZE] = {0x00}; 
    unsigned int8 packetdata_index = 0;

    /*
    // Header creation
    packetdata[0] = START_MAKER;
    packetdata[1] = (START_MAKER & 0xF0) | (port_data_ptr->port_num & 0x0F);
    packetdata[2] = measured_data_ptr->time >> 24 & 0xFF;
    packetdata[3] = measured_data_ptr->time >> 16 & 0xFF;
    packetdata[4] = measured_data_ptr->time >> 8 & 0xFF;
    packetdata[5] = measured_data_ptr->time_sec & 0xFF;
    packetdata[6] = (measured_data_ptr->time_msec & 0x0F) >>4 | measured_data_ptr->pd >> 8 & 0x0F;
    packetdata[7] = measured_data_ptr->pd & 0xFF;
    packetdata[8] = (measured_data_ptr->open_voltage >> 4) & 0xFF; // PD start low byte
    packetdata[9] = (measured_data_ptr->open_voltage & 0x0F) << 4 | (measured_data_ptr->temp_py_top >> 8) & 0x0F;
    packetdata[10] = measured_data_ptr->temp_py_top & 0xFF;
    packetdata[11] = (measured_data_ptr->temp_py_bot >> 4) & 0xFF; // PD start low byte
    packetdata[12] = (measured_data_ptr->temp_py_bot & 0x0F) << 4 | (measured_data_ptr->temp_mis7 >> 8) & 0x0F;
    packetdata[13] = measured_data_ptr->temp_mis7 & 0xFF;
    */
    packetdata_index = 14; // Start index for data

    for (unsigned int16 i = 0; i < port_data_ptr->sweep_step ; i++) {
        unsigned int16 data0 = port_data_ptr->data_buffer[0][i];
        unsigned int16 data1 = port_data_ptr->data_buffer[1][i];

        packetdata[packetdata_index++] = (data0 >> 4) & 0xFF;
        packetdata[packetdata_index++] = ((data0 & 0x0F) << 4) | ((data1 >> 8) & 0x0F);
        packetdata[packetdata_index++] = data1 & 0xFF;

        if (packetdata_index + 3 >= PACKET_SIZE - 1) {
            packetdata[PACKET_SIZE - 1] = calc_crc8(packetdata, PACKET_SIZE - 1);
           unsigned int32 write_address;
            
            switch (port_data_ptr -> port_num) {
                case 1:
                    write_address = MISF_CIGS_IV_DATA_START + iv1_data_ptr->used_counter;
                    write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
                    iv1_data_ptr->used_counter += PACKET_SIZE;
                    iv1_data_ptr->uncopied_counter += PACKET_SIZE;
                    break;
                case 2:
                    write_address = MISF_CIGS_IV_DATA_START + iv2_data_ptr->used_counter;
                    write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
                    iv2_data_ptr->used_counter += PACKET_SIZE;
                    iv2_data_ptr->uncopied_counter += PACKET_SIZE;
                    break;
                default:
                    break;
            }
            packetdata_index = 0;
            memset(packetdata, 0x00, PACKET_SIZE);
        }
    }

    // 最後に残ったデータの処理
    if (packetdata_index > 0) {
        for (unsigned int8 j = packetdata_index; j < PACKET_SIZE - 1; j++) {
            packetdata[j] = 0x00;
        }
        packetdata[PACKET_SIZE - 1] = calc_crc8(packetdata, PACKET_SIZE - 1);
      unsigned int32 write_address;
        switch (port_data_ptr->port_num)
        {
        case 1:
            write_address = MISF_CIGS_IV_DATA_START + iv1_data_ptr->used_counter;
            write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
            iv1_data_ptr->used_counter += PACKET_SIZE;
            iv1_data_ptr->uncopied_counter += PACKET_SIZE;
            break;
        case 2:
         write_address = MISF_CIGS_IV_DATA_START + iv2_data_ptr->used_counter;
            write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
            iv2_data_ptr->used_counter += PACKET_SIZE;
            iv2_data_ptr->uncopied_counter += PACKET_SIZE;
            break;
        default:
            break;
        }

    }
    // fprintf(PC, "End CIGS data conversion\r\n");
    write_misf_address_area(); // Write the mission flash address area
}

void log_meas_data_with_print(iv_env_t *measured_data_ptr, sweep_config_t *port_data_ptr)
{
    unsigned int8 packetdata[PACKET_SIZE] = {0x00}; 
    unsigned int8 packetdata_index = 0;
    int1 first_packet = true; // 最初のパケットフラグ

    // ==== データステップ書き込み ====
    for (unsigned int16 i = 0; i < port_data_ptr->sweep_step; i++) {
        unsigned int16 data0 = port_data_ptr->data_buffer[0][i];
        unsigned int16 data1 = port_data_ptr->data_buffer[1][i];

        // パケットがいっぱいになる前にヘッダ＋環境データを入れる
        if (packetdata_index == 0) {
            unsigned int8 idx = 0;
            if (first_packet) {
                packetdata[idx++] = START_MAKER;  // 最初のパケットだけ
                first_packet = false;
            }

            // timeヘッダ
            packetdata[idx++] = (measured_data_ptr->time >> 24) & 0xFF;
            packetdata[idx++] = (measured_data_ptr->time >> 16) & 0xFF;
            packetdata[idx++] = (measured_data_ptr->time >> 8) & 0xFF;
            packetdata[idx++] = measured_data_ptr->time & 0xFF;

            // 環境データ
            packetdata[idx++] = (measured_data_ptr->pd >> 4) & 0xFF;
            packetdata[idx++] = ((measured_data_ptr->pd & 0x0F) << 4) | ((measured_data_ptr->temp_py_top >> 8) & 0x0F);
            packetdata[idx++] = measured_data_ptr->temp_py_top & 0xFF;
            packetdata[idx++] = (measured_data_ptr->temp_py_bot >> 4) & 0xFF;
            packetdata[idx++] = ((measured_data_ptr->temp_py_bot & 0x0F) << 4) | ((measured_data_ptr->temp_mis7 >> 8) & 0x0F);
            packetdata[idx++] = measured_data_ptr->temp_mis7 & 0xFF;

            packetdata_index = idx;
        }

        // パケットに収まるかチェック
        if (packetdata_index + DATA_SIZE_PER_STEP >= PACKET_SIZE) {
            // CRC 計算
            packetdata[PACKET_SIZE - 1] = calc_crc8(packetdata, PACKET_SIZE - 1);

            // フラッシュ書き込み
            unsigned int32 write_address = MISF_CIGS_IV_DATA_START;
            switch (port_data_ptr->port_num) {
                case 1:
                    write_address += iv1_data_ptr->used_counter;
                    write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
                    iv1_data_ptr->used_counter += PACKET_SIZE;
                    iv1_data_ptr->uncopied_counter += PACKET_SIZE;
                    break;
                case 2:
                    write_address += iv2_data_ptr->used_counter;
                    write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
                    iv2_data_ptr->used_counter += PACKET_SIZE;
                    iv2_data_ptr->uncopied_counter += PACKET_SIZE;
                    break;
                default:
                    break;
            }

            // デバッグ表示
            fprintf(PC, "\r\n");
            for (unsigned int32 j = 0; j < PACKET_SIZE; j++) {
                fprintf(PC, "%02X ", packetdata[j]);
            }

            // パケット初期化
            packetdata_index = 0;
            memset(packetdata, 0x00, PACKET_SIZE);
        }

        // データ書き込み
        packetdata[packetdata_index++] = (data0 >> 4) & 0xFF;
        packetdata[packetdata_index++] = ((data0 & 0x0F) << 4) | ((data1 >> 8) & 0x0F);
        packetdata[packetdata_index++] = data1 & 0xFF;
    }

    // 最後に残ったデータの処理
    if (packetdata_index > 0) {
        for (unsigned int8 j = packetdata_index; j < PACKET_SIZE - 1; j++) {
            packetdata[j] = 0x00;
        }
        packetdata[PACKET_SIZE - 1] = calc_crc8(packetdata, PACKET_SIZE - 1);
      unsigned int32 write_address;
        switch (port_data_ptr->port_num)
        {
        case 1:
            write_address = MISF_CIGS_IV_DATA_START + iv1_data_ptr->used_counter;
            write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
            iv1_data_ptr->used_counter += PACKET_SIZE;
            iv1_data_ptr->uncopied_counter += PACKET_SIZE;
            break;
        case 2:
         write_address = MISF_CIGS_IV_DATA_START + iv2_data_ptr->used_counter;
            write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
            iv2_data_ptr->used_counter += PACKET_SIZE;
            iv2_data_ptr->uncopied_counter += PACKET_SIZE;
            break;
        default:
            break;
        }

    }
    // fprintf(PC, "End CIGS data conversion\r\n");
    write_misf_address_area(); // Write the mission flash address area
}


iv_env_t create_meas_data()
{
    iv_env_t data;
    memset(&data, 0, sizeof(data));
    data.time           = get_current_sec();
    data.temp_py_top    = ad7490_read(ADC_TEMP_PY_TOP);
    data.temp_py_bot    = ad7490_read(ADC_TEMP_PY_BOT);
    data.temp_mis7      = ad7490_read(ADC_TEMP_MIS7);
    data.pd             = ad7490_read(ADC_PD);
    return data;
}
// End of file
