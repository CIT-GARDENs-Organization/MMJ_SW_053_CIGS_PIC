#include "mmj_cigs_iv.h"              
#include "../hal/mmj_cigs_config.h"      
#include "../device_driver/ad7490_driver.h"  
#include "../device_driver/mcp4901_driver.h"
#include "mmj_cigs_flash.h"               
#include "../lib/communication/mission_tools.h"   
#include "../hal/timer.h"

void sweep_with_print()
{
    unsigned int16 curr_threshold = 300;
    unsigned int16 curr_limit = 700;

    // Enable both CIGS ports
    connect_port1();
    connect_port2();
    mcp4901_1_write(0); 
    mcp4901_2_write(0);
    delay_ms(100);
    
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
    iv_env_t measured_data = create_meas_data();

    while (port1.active || port2.active)
    {
        // 出力設定
        if (port1.active) {
            mcp4901_1_write(count);
        } else {
            mcp4901_1_write(0);  // 閾値到達後は0出力
        }

        if (port2.active) {
            mcp4901_2_write(count);
        } else {
            mcp4901_2_write(0);
        }

        delay_us(100);

        // データ取得
        if (port1.active) {
            port1.data_buffer[count].voltage = ad7490_read(ADC_CIGS1_AMP);
            port1.data_buffer[count].current = ad7490_read(ADC_CIGS1_CURR);
            port1.sweep_step = count + 1;
            if (port1.data_buffer[count].current < curr_limit) {
                port1.active = 0;  // 閾値到達で測定終了
            }
        }

        if (port2.active) {
            port2.data_buffer[count].voltage = ad7490_read(ADC_CIGS2_AMP);
            port2.data_buffer[count].current = ad7490_read(ADC_CIGS2_CURR);
            port2.sweep_step = count + 1;
            if (port2.data_buffer[count].current < curr_limit) {
                port2.active = 0;
            }
        }

        count++;
        if (count >= 255) {
            break;
        }
    }

    // Print I-V data
    fprintf(PC, "Volt1,Curr1,Volt2,Curr2\r\n");
    for (int i = 0; i < count; i++) {

        fprintf(PC, "%04LX,%04LX,%04LX,%04LX\r\n",
            port1.data_buffer[i].voltage, port1.data_buffer[i].current,
            port2.data_buffer[i].voltage, port2.data_buffer[i].current);
    }
}



void test_sweep(unsigned int16 curr_threshold, unsigned int16 curr_limit)
{
    fprintf(PC, ".");
    
    
    connect_port1();
    connect_port2();
    // Enable both CIGS ports
    mcp4901_1_write(0); 
    mcp4901_2_write(0);
    connect_port1();
    connect_port2();

    // Stabilization wait
    delay_ms(100);
    
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
    iv_env_t measured_data = create_meas_data();
    while (port1.active || port2.active)
    {
        mcp4901_1_write(count);
        mcp4901_2_write(count);
        delay_us(10); 
        if (port1.active) {
            unsigned int32 ans = ad7490_read(ADC_CIGS1_CURR);
            ans += ad7490_read(ADC_CIGS1_CURR);
            ans = ans/2;
            port1.data_buffer[count].voltage = ad7490_read(ADC_CIGS1_AMP);
            port1.data_buffer[count].current = ans;
            port1.sweep_step = count + 1; 
            if (port1.data_buffer[count].current < curr_limit) {
                port1.active = 0;
                disconnect_port1();
            }
        }
        if (port2.active) {
            port2.data_buffer[count].voltage = ad7490_read(ADC_CIGS2_AMP);
            port2.data_buffer[count].current = ad7490_read(ADC_CIGS2_CURR);
            port2.sweep_step = count + 1;
            if (port2.data_buffer[count].current < curr_limit) {
                port2.active = 0;
                disconnect_port2();
            } 
        }
        count++;
        if (count >= 255) {
            // fprintf(PC, "Maximum step count reached: %ld\r\n", count);
            break;
        }
    }

    for (int i = 0; i < count; i++) {

        fprintf(PC, "%04LX,%04LX,%04LX,%04LX\r\n",
            port1.data_buffer[i].voltage, port1.data_buffer[i].current,
            port2.data_buffer[i].voltage, port2.data_buffer[i].current);
    }

}

void sweep(unsigned int16 curr_threshold, unsigned int16 curr_limit, unsigned int16 pd_limit)
{
    fprintf(PC, ".");
    // Enable both CIGS ports
    connect_port1();
    connect_port2();

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

    int16 count = 0;
    
    // Initialize DACs to 0
    mcp4901_1_write(1);
    mcp4901_2_write(1);

    iv_env_t measured_data = create_meas_data();
    while (port1.active || port2.active)
    {
        mcp4901_1_write(count);
        mcp4901_2_write(count);
        delay_us(10); 
        if (port1.active) {
            port1.data_buffer[count].voltage = ad7490_read(ADC_CIGS1_AMP);
            port1.data_buffer[count].current = ad7490_read(ADC_CIGS1_CURR);
            port1.sweep_step = count + 1; 
            // fprintf(PC, "%04LX,%04LX,", port1.data_buffer[count].voltage, port1.data_buffer[count].current);
            if (port1.data_buffer[count].current < curr_limit) {
                port1.active = 0;
                disconnect_port1();
            }
        }
        if (port2.active) {
            port2.data_buffer[count].voltage = ad7490_read(ADC_CIGS2_AMP);
            port2.data_buffer[count].current = ad7490_read(ADC_CIGS2_CURR);
            port2.sweep_step = count + 1;
            if (port2.data_buffer[count].current < curr_limit) {
                port2.active = 0;
                disconnect_port2();
            } 
        }
        count++;
        if (count >= 255) {
            // fprintf(PC, "Maximum step count reached: %ld\r\n", count);
            break;
        }
    }

    // Ensure all connections are disabled
    disconnect_port1();
    disconnect_port2();
    log_meas_data(&measured_data, &port1);
    log_meas_data(&measured_data, &port2);
}


void log_meas_data(iv_env_t *measured_data_ptr, sweep_config_t *port_data_ptr)
{
    iv_data_packet_t data_packet = {0};
    iv_data_packet_t *data_packet_ptr = &data_packet;
    
    // ヘッダ情報の設定n
    data_packet.header.start_marker = START_MAKER;
    data_packet.header.time_sec = measured_data_ptr->time / 1000;
    data_packet.header.time_msec = measured_data_ptr->time % 1000;

    // 環境データの設定
    data_packet.header.env_data[0].data[0] = (measured_data_ptr->pd >> 4) & 0xFF;
    data_packet.header.env_data[0].data[1] = ((measured_data_ptr->pd & 0x0F) << 4) | ((measured_data_ptr->temp_py_top >> 8) & 0x0F);
    data_packet.header.env_data[0].data[2] = measured_data_ptr->temp_py_top & 0xFF;
    data_packet.header.env_data[1].data[0] = (measured_data_ptr->temp_py_bot >> 4) & 0xFF;
    data_packet.header.env_data[1].data[1] = ((measured_data_ptr->temp_py_bot & 0x0F) << 4) | ((measured_data_ptr->temp_mis7 >> 8) & 0x0F);
    data_packet.header.env_data[1].data[2] = measured_data_ptr->temp_mis7 & 0xFF;
    
    // IVデータの1パケット目
    unsigned int16 iv_index = 0;
    for (unsigned int16 i = 0; i < 16 && iv_index < port_data_ptr->sweep_step; i++, iv_index++){
        data_packet.header.iv_data[i].data[0] = (port_data_ptr->data_buffer[i].voltage >> 4) & 0xFF;
        data_packet.header.iv_data[i].data[1] = ((port_data_ptr->data_buffer[i].voltage & 0x0F) << 4) | ((port_data_ptr->data_buffer[i].current >> 8) & 0x0F);
        data_packet.header.iv_data[i].data[2] = port_data_ptr->data_buffer[i].current & 0xFF;
    }
    switch (port_data_ptr -> port_num) {
        case 1:
            misf_write_data(FLASH_ID_IV1_DATA, data_packet_ptr->raw, PACKET_SIZE-1);
            break;
        case 2:
            misf_write_data(FLASH_ID_IV2_DATA, data_packet_ptr->raw, PACKET_SIZE-1);
            break;
        default:
            break;
    }

    // 2パケット目以降のIVデータ
    while (iv_index < port_data_ptr->sweep_step) {
        // 新しいパケットの初期化（ゼロクリア）
        memset(data_packet_ptr->raw, 0x00, sizeof(data_packet.raw));

        unsigned int16 step_in_packet = 0;

        // データを詰める
        for (; step_in_packet < 21 && iv_index < port_data_ptr->sweep_step;
            step_in_packet++, iv_index++) {
            data_packet.data.iv_data[step_in_packet].data[0] =
                (port_data_ptr->data_buffer[iv_index].voltage >> 4) & 0xFF;
            data_packet.data.iv_data[step_in_packet].data[1] =
                ((port_data_ptr->data_buffer[iv_index].voltage & 0x0F) << 4) |
                ((port_data_ptr->data_buffer[iv_index].current >> 8) & 0x0F);
            data_packet.data.iv_data[step_in_packet].data[2] =
                port_data_ptr->data_buffer[iv_index].current & 0xFF;
        }

        // 残り領域を 0x00 で埋める（未使用部分）
        for (; step_in_packet < 21; step_in_packet++) {
            data_packet.data.iv_data[step_in_packet].data[0] = 0x00;
            data_packet.data.iv_data[step_in_packet].data[1] = 0x00;
            data_packet.data.iv_data[step_in_packet].data[2] = 0x00;
        }

        // フラッシュに書き込み
        if (port_data_ptr->port_num == 1) {
            misf_write_data(FLASH_ID_IV1_DATA, data_packet_ptr->raw, PACKET_SIZE - 1);
        } else if (port_data_ptr->port_num == 2) {
            misf_write_data(FLASH_ID_IV2_DATA, data_packet_ptr->raw, PACKET_SIZE - 1);
        }
    }
}

void log_meas_data_with_print(iv_env_t *measured_data_ptr, sweep_config_t *port_data_ptr)
{
    unsigned int8 packetdata[PACKET_SIZE] = {0x00}; 
    unsigned int8 packet_index = 0;
    int1 first_packet = true; // 最初のパケットフラグ


    unsigned int16 sweep_index = 0;
    // ==== データステップ書き込み ====
    for (unsigned int16 i = 0; i < port_data_ptr->sweep_step; i++) {
        unsigned int16 data0 = port_data_ptr->data_buffer[i].voltage;
        unsigned int16 data1 = port_data_ptr->data_buffer[i].current;

        // パケットがいっぱいになる前にヘッダ＋環境データを入れる
        if (packet_index == 0) {
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

            packet_index = idx;
        }

        // パケットに収まるかチェック
        if (packet_index + DATA_SIZE_PER_STEP >= PACKET_SIZE) {
            // フラッシュ書き込み
            unsigned int32 write_address = MISF_CIGS_IV1_DATA_START;
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
            packet_index = 0;
            memset(packetdata, 0x00, PACKET_SIZE);
        }

        // データ書き込み
        packetdata[packet_index++] = (data0 >> 4) & 0xFF;
        packetdata[packet_index++] = ((data0 & 0x0F) << 4) | ((data1 >> 8) & 0x0F);
        packetdata[packet_index++] = data1 & 0xFF;
    }

    // 最後に残ったデータの処理
    if (packet_index > 0) {
        for (unsigned int8 j = packet_index; j < PACKET_SIZE - 1; j++) {
            packetdata[j] = 0x00;
        }
        packetdata[PACKET_SIZE - 1] = calc_crc8(packetdata, PACKET_SIZE - 1);
      unsigned int32 write_address;
        switch (port_data_ptr->port_num)
        {
        case 1:
            write_address = MISF_CIGS_IV1_DATA_START + iv1_data_ptr->used_counter;
            write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
            iv1_data_ptr->used_counter += PACKET_SIZE;
            iv1_data_ptr->uncopied_counter += PACKET_SIZE;
            break;
        case 2:
         write_address = MISF_CIGS_IV2_DATA_START + iv2_data_ptr->used_counter;
            write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
            iv2_data_ptr->used_counter += PACKET_SIZE;
            iv2_data_ptr->uncopied_counter += PACKET_SIZE;
            break;
        default:
            break;
        }

    }
                // デバッグ表示
    fprintf(PC, "\r\n");
    for (unsigned int32 j = 0; j < PACKET_SIZE; j++) {
        fprintf(PC, "%02X ", packetdata[j]);
    }
    // fprintf(PC, "End CIGS data conversion\r\n");
    misf_update_address_area(); // Write the mission flash address area
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

unsigned int16 calc_pd_value(unsigned int16 data)
{
    // ADCの12ビット値を電力密度（mW/cm²）に変換する例
    // ここでは仮に1 LSB = 0.1 mW/cm²とする
    return (unsigned int16)(data * 0.1);
}

unsigned int16 calc_curr_value(unsigned int16 data)
{
    float voltage_mv = (float)(data * ADC_REF_VOLTAGE_MV / ADC_MAX_READING); // 12ビットADCの場合
    float current_ma = (ADC_CURR_REF_VOLTAGE_MV - voltage_mv) / (SHUNT_RESISTANCE_OHM * AMP_GAIN); // mA単位
    return (unsigned int16)current_ma;
}
// End of file
