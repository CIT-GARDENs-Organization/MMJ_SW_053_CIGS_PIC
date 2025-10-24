#include "cigs_iv.h"      
#include "../hal/cigs_config.h"      
#include "../device_driver/ad7490_driver.h"  
#include "../device_driver/mcp4901_driver.h"
#include "cigs_flash.h"               
#include "../lib/communication/mission_tools.h"   
#include "../hal/timer.h"
#include "../lib/device/mt25q.h"

void sweep_with_print()
{
    unsigned int16 curr_threshold = 0x300;
    unsigned int16 curr_limit = 0x7E0;

    // Enable both CIGS ports
    connect_port1();
    connect_port2();
    mcp4901_1_write(0); 
    mcp4901_2_write(0);
    delay_ms(100);
    
    // Init Port1
    sweep_debug_config_t port1 = {0};
    port1.port_num = 1;
    port1.sweep_step = 0;
    port1.active = 1;

    // Init Port2
    sweep_debug_config_t port2 = {0};
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

    fprintf(PC, "temp_top:%04LX\r\n", measured_data.temp_py_top);
    fprintf(PC, "temp_bot:%04LX\r\n", measured_data.temp_py_bot);
    fprintf(PC, "temp_mis7:%04LX\r\n", measured_data.temp_mis7);
    fprintf(PC, "pd:%04LX\r\n", measured_data.pd);
    // Print I-V data
    fprintf(PC, "Volt1,Curr1,Volt2,Curr2\r\n");
    for (int i = 0; i < count; i++) {

        fprintf(PC, "%04LX,%04LX,%04LX,%04LX\r\n",
            port1.data_buffer[i].voltage, port1.data_buffer[i].current,
            port2.data_buffer[i].voltage, port2.data_buffer[i].current);
    }
    log_meas_data(&measured_data, &port1);
    log_meas_data(&measured_data, &port2);
    misf_update_address_area();
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
    sweep_debug_config_t port1 = {0};
    port1.port_num = 1;
    port1.sweep_step = 0;
    port1.active = 1;

    // Init Port2
    sweep_debug_config_t port2 = {0};
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
    unsigned int32 start_time_ms = get_current_sec();
    fputc('.', PC);
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

    unsigned int16 volt;
    unsigned int16 curr;
    iv_env_t measured_data = create_meas_data();
    while (port1.active || port2.active)
    {
        mcp4901_1_write(count);
        mcp4901_2_write(count);
        delay_us(1); 
        if (port1.active) {
            volt = ad7490_read(ADC_CIGS1_AMP);
            curr = ad7490_read(ADC_CIGS1_CURR);
            // ad7490_read_2port(ADC_CIGS1_AMP, ADC_CIGS1_CURR, &volt, &curr);
            // fprintf(PC, "%04LX,%04LX,", volt, curr);
            port1.data_buffer[count*3]= (volt  >> 4) & 0xFF;
            port1.data_buffer[count*3+1]= ((volt & 0x0F) << 4) | ((curr >> 8) & 0x0F);
            port1.data_buffer[count*3+2]= curr & 0xFF;
            port1.sweep_step = count + 1; 
            // fprintf(PC, "%04LX,%04LX,", port1.data_buffer[count].voltage, port1.data_buffer[count].current);
            if (curr< curr_limit) {
                port1.active = 0;
                disconnect_port1();
            }
        }
        if (port2.active) {
            volt = ad7490_read(ADC_CIGS2_AMP);
            curr = ad7490_read(ADC_CIGS2_CURR);
            port2.data_buffer[count*3]= (volt  >> 4) & 0xFF;
            port2.data_buffer[count*3+1]= ((volt & 0x0F) << 4) | ((curr >> 8) & 0x0F);
            port2.data_buffer[count*3+2]= curr & 0xFF;
            port2.sweep_step = count + 1;
            if (curr < curr_limit) {
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
    // unsigned int32 end_time_ms = get_current_msec();
    // Ensure all connections are disabled3
    disconnect_port1();
    disconnect_port2();
    log_meas_data(&measured_data, &port1);
    log_meas_data(&measured_data, &port2);
    // misf_update_address_area();
    unsigned int32 end_time_ms = get_current_msec();
    unsigned int32 elapsed_time_ms = end_time_ms - start_time_ms;
}


void sweep_port1(unsigned int16 curr_limit)
{
    unsigned int32 start_time_ms = get_current_sec();
    fputc('.', PC);
    // Enable both CIGS ports
    connect_port1();

    // Init Port1
    sweep_config_t port1 = {0};
    port1.port_num = 1;
    port1.sweep_step = 0;
    port1.active = 1;

    int16 count = 0;
    
    // Initialize DACs to 0
    mcp4901_1_write(1);
    unsigned int16 volt;
    unsigned int16 curr;
    iv_env_t measured_data = create_meas_data();
    while (port1.active)
    {
        mcp4901_1_write(count);
        // mcp4901_2_write(count);
        delay_us(1); 
        if (port1.active) {
            volt = ad7490_read(ADC_CIGS1_AMP);
            curr = ad7490_read(ADC_CIGS1_CURR);
            // ad7490_read_2port(ADC_CIGS1_AMP, ADC_CIGS1_CURR, &volt, &curr);
            // fprintf(PC, "%04LX,%04LX,", volt, curr);
            port1.data_buffer[count*3]= (volt  >> 4) & 0xFF;
            port1.data_buffer[count*3+1]= ((volt & 0x0F) << 4) | ((curr >> 8) & 0x0F);
            port1.data_buffer[count*3+2]= curr & 0xFF;
            port1.sweep_step = count + 1; 
            // fprintf(PC, "%04LX,%04LX,", port1.data_buffer[count].voltage, port1.data_buffer[count].current);
            if (curr< curr_limit) {
                port1.active = 0;
                disconnect_port1();
            }
        }
        count++;
        if (count >= 255) {
            // fprintf(PC, "Maximum step count reached: %ld\r\n", count);
            break;
        }
    }
    // unsigned int32 end_time_ms = get_current_msec();
    // Ensure all connections are disabled3
    disconnect_port1();
    log_meas_data(&measured_data, &port1);
}




void log_meas_data(iv_env_t *measured_data_ptr, sweep_config_t *port_data_ptr)
{
    iv_data_packet_t data_packet = {0};
    iv_data_packet_t *data_packet_ptr = &data_packet;

    // ========================ヘッダーパケットの処理========================
    // ヘッダ情報の設定
    data_packet.header.start_marker = START_MAKER;
    data_packet.header.time_sec  = measured_data_ptr->time;
    data_packet.header.time_msec = (unsigned int8)((measured_data_ptr->time >> 4) & 0xFF);

    // 環境データの設定
    data_packet.header.env_data[0].data[0] = (measured_data_ptr->pd >> 4) & 0xFF;
    data_packet.header.env_data[0].data[1] =
        ((measured_data_ptr->pd & 0x0F) << 4) |
        ((measured_data_ptr->temp_py_top >> 8) & 0x0F);
    data_packet.header.env_data[0].data[2] = measured_data_ptr->temp_py_top & 0xFF;

    data_packet.header.env_data[1].data[0] = (measured_data_ptr->temp_py_bot >> 4) & 0xFF;
    data_packet.header.env_data[1].data[1] =
        ((measured_data_ptr->temp_py_bot & 0x0F) << 4) |
        ((measured_data_ptr->temp_mis7 >> 8) & 0x0F);
    data_packet.header.env_data[1].data[2] = measured_data_ptr->temp_mis7 & 0xFF;

    // I-Vデータの設定
    for (unsigned int8 i = 0; i < IV_HEADER_SIZE; i++) {
        if (i < port_data_ptr->sweep_step) {
            data_packet.header.ivdata[i*3]     = port_data_ptr->data_buffer[i*3];
            data_packet.header.ivdata[i*3 + 1] = port_data_ptr->data_buffer[i*3 + 1];
            data_packet.header.ivdata[i*3 + 2] = port_data_ptr->data_buffer[i*3 + 2];
        } else {
            // 余りは0埋め
            data_packet.header.ivdata[i*3]     = 0;
            data_packet.header.ivdata[i*3 + 1] = 0;
            data_packet.header.ivdata[i*3 + 2] = 0;
        }
    }
    // フラッシュに書き込み
    if (port_data_ptr->port_num == 1) {
        data_packet.header.crc = calc_crc8(data_packet_ptr->raw, PACKET_SIZE - 1);
        misf_write_data(FLASH_ID_IV1_DATA, data_packet_ptr->raw, PACKET_SIZE);
        // for (int i = 0; i < PACKET_SIZE; i++) {
            // fprintf(PC, "%02X ", data_packet_ptr->raw[i]);
        // }
        unsigned int8 read_back[PACKET_SIZE];
        // read_data_bytes(mis_fm, MISF_ADDRESS_TABLE[FLASH_ID_IV1_DATA].start, read_back, PACKET_SIZE);
        // for (int j = 0; j < PACKET_SIZE; j++) {
            // fprintf(PC, "%02X ", read_back[j]);
        // }
    } else if (port_data_ptr->port_num == 2) {
        data_packet.header.crc = calc_crc8(data_packet_ptr->raw, PACKET_SIZE - 1);
        misf_write_data(FLASH_ID_IV2_DATA, data_packet_ptr->raw, PACKET_SIZE);
    }

    // ========================データパケットの処理========================
    unsigned int16 iv_index = IV_HEADER_SIZE;
    // パケットごとに繰り返す
    while (iv_index < port_data_ptr->sweep_step) {
        unsigned int16 step_in_packet = 0;

        // データを詰める（最大21点/パケット）
        for (; step_in_packet < IV_DATA_SIZE && iv_index < port_data_ptr->sweep_step;
             step_in_packet++, iv_index++) {
            data_packet.data.iv_data[step_in_packet*3] = port_data_ptr->data_buffer[iv_index*3];
            data_packet.data.iv_data[step_in_packet*3+1] = port_data_ptr->data_buffer[iv_index*3+1];
            data_packet.data.iv_data[step_in_packet*3+2] = port_data_ptr->data_buffer[iv_index*3+2];
        }

        // 残り領域を 0x00 で埋める（未使用部分）
        for (; step_in_packet < 21; step_in_packet++) {
            data_packet.data.iv_data[step_in_packet*3]  = 0x00;
            data_packet.data.iv_data[step_in_packet*3+1] = 0x00;
            data_packet.data.iv_data[step_in_packet*3+2] = 0x00;
        }

        // フラッシュに書き込み
        if (port_data_ptr->port_num == 1) {
            data_packet.data.crc = calc_crc8(data_packet_ptr->raw, PACKET_SIZE - 1);
            misf_write_data(FLASH_ID_IV1_DATA, data_packet_ptr->raw, PACKET_SIZE);
            // for (int i = 0; i < PACKET_SIZE; i++) {
                // fprintf(PC, "%02X ", data_packet_ptr->raw[i]);
            // }
        } else if (port_data_ptr->port_num == 2) {
            data_packet.data.crc = calc_crc8(data_packet_ptr->raw, PACKET_SIZE - 1);
            misf_write_data(FLASH_ID_IV2_DATA, data_packet_ptr->raw, PACKET_SIZE);
        }
    }

    // misf_update_address_area(); // 必要なら有効化
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
    return (unsigned int16)(data);
}

int16 calc_curr_value(unsigned int16 data){
    float voltage_mv;
    float current_ma;

    // 12ビットADCの値を[mV]に変換
    voltage_mv = (float)data * ADC_REF_VOLTAGE_MV / ADC_MAX_READING;

    // オフセット電圧(ADC_CURR_REF_VOLTAGE_MV)との差から電流[mA]を算出
    current_ma = (voltage_mv - ADC_CURR_REF_VOLTAGE_MV) / (SHUNT_RESISTANCE_OHM * AMP_GAIN);

    return (int16)current_ma;
}
// End of file
