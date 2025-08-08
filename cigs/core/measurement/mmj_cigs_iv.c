#include "mmj_cigs_iv.h"               // 同じフォルダのヘッダー
#include "../../system/mmj_cigs_config.h"      // システム設定
#include "../../hardware/devices/ad7490_driver.h"   // デバイス定義  
#include "../../hardware/devices/mcp4901_driver.h"
#include "../storage/mmj_cigs_flash.h"               // ストレージ機能
#include "../../../lib/communication/mission_tools.h"   // 通信ツール
#include "../../hardware/mcu/timer.h"

void io_init()
{
    fprintf(PC, "IO Initialize\r\n");
    // output_high(ADC_CS); // ADC Chip Select
    output_high(MIS_FM_CS); // DAC Chip Select
    output_high(SMF_CS); // SMF Chip Select
    output_low(CONNECT_CIGS1);
    output_low(CONNECT_CIGS2);
    output_high(EN_NPWR);
    delay_ms(1);
    fprintf(PC, "\tComplete\r\n");
}


void test_sweep(unsigned int8 sweep_step)
{
    fprintf(PC, "Start TEST SWEEP\r\n");
    output_high(CONNECT_CIGS1);
    output_low(EN_NPWR); // Enable power to CIGS
    delay_us(100); // wait for the CIGS to stabilize
    fprintf(PC, "step, voltage, current\r\n");
    mcp4901_1_write(0); // Initialize DAC to 0
    delay_ms(100); // wait for the DAC to stabilize


    unsigned int16 volt;
    unsigned int16 curr;
    for (unsigned int8 count = 0; count < sweep_step; count++)
    {    
        // set DAC value
        mcp4901_1_write(count);

        volt = ad7490_read(ADC_CIGS1_AMP); // read CIGS voltage
        curr = ad7490_read(ADC_CIGS1_CURR); // read CIGS current
        
        fprintf(PC, "%u, %lu, %lu\r\n", count, volt, curr);
        delay_ms(1); // wait for the ADC to stabilize
    }
}

void test_adc()
{
    unsigned int16 ans;
    mcp4901_1_write(0); // Initialize DAC to 0
    fprintf(PC, "Start ADC TEST\r\n");
    ans = ad7490_read(ADC_CIGS1_VOLT);
    fprintf(PC, "ADC Voltage: %04LX\r\n", ans);
    ans = ad7490_read(ADC_CIGS1_CURR);
    fprintf(PC, "ADC Current: %04LX\r\n", ans);
    ans = ad7490_read(ADC_CIGS1_AMP);
    fprintf(PC, "ADC CIGS1 Amp: %04LX\r\n", ans);
    ans = ad7490_read(ADC_CIGS1_VREF);
    fprintf(PC, "ADC CIGS1 VREF: %04LX\r\n", ans);
}

void sweep_2ports(unsigned int8 measurement_step)
{ 
    fprintf(PC, "Start SWEEP 2ports\r\n");
    fprintf(PC, "\tSweep step : %u\r\n", measurement_step);
    
    // Set CIGS1 and CIGS2
    output_high(CONNECT_CIGS1);
    output_high(CONNECT_CIGS2);
    output_low(EN_NPWR); 

    MEASUREMENT_DATA measured_data;

    measured_data.time_sec = get_current_sec(); 
    measured_data.temp_py_top = ad7490_read(ADC_TEMP_PY_TOP);
    measured_data.temp_py_bot = ad7490_read(ADC_TEMP_PY_BOT);
    measured_data.temp_mis7 = ad7490_read(ADC_TEMP_MIS7);
    measured_data.pd = ad7490_read(ADC_PD); 


    unsigned int16 cigs1_buffer[2][0xFF]; // Buffer for CIGS1 data
    unsigned int16 cigs2_buffer[2][0xFF];


    for (unsigned int8 count = 0; count < measurement_step; count++)
    {    
        // set DAC value
        mcp4901_1_write(count);
        mcp4901_2_write(count);
        //delay_ms(100); // wait for the DAC to stabilize

        // read CIGS voltage and current      
        delay_ms(1);
        cigs1_buffer[0][count] = ad7490_read(ADC_CIGS1_VOLT);
        cigs1_buffer[1][count] = ad7490_read(ADC_CIGS1_CURR);
        cigs2_buffer[0][count] = ad7490_read(ADC_CIGS2_VOLT);
        cigs2_buffer[1][count] = ad7490_read(ADC_CIGS2_CURR);
    }

    output_low(CONNECT_CIGS1);
    output_low(CONNECT_CIGS2);
    output_high(EN_NPWR);

    fprintf(PC, "END SWEEP 2port\r\n");

    fprintf(PC, "Start CIGS data conversion\r\n");  
}

void sweep_port1(unsigned int8 measurement_step)
{
    fprintf(PC, "Start SWEEP PORT1\r\n");
    fprintf(PC, "\tSweep step : %u\r\n", measurement_step);
    
    // Set CIGS1
    output_high(CONNECT_CIGS1);
    output_low(CONNECT_CIGS2);
    output_low(EN_NPWR); 

    MEASUREMENT_DATA measured_data;

    measured_data.time_sec = get_current_sec(); 
    measured_data.temp_py_top = ad7490_read(ADC_TEMP_PY_TOP);
    measured_data.temp_py_bot = ad7490_read(ADC_TEMP_PY_BOT);
    measured_data.temp_mis7 = ad7490_read(ADC_TEMP_MIS7);
    measured_data.pd = ad7490_read(ADC_PD); 

    unsigned int16 cigs1_buffer[2][0xFF]; // Buffer for CIGS1 data

    for (unsigned int8 count = 0; count < measurement_step; count++)
    {    
        // set DAC value
        mcp4901_1_write(count);
        //delay_ms(100); // wait for the DAC to stabilize

        // read CIGS voltage and current      
        delay_ms(10);
        cigs1_buffer[0][count] = ad7490_read(ADC_CIGS1_AMP);
        cigs1_buffer[1][count] = ad7490_read(ADC_CIGS1_CURR);
    }
    



    output_low(CONNECT_CIGS1);
    output_high(EN_NPWR);

    fprintf(PC, "END SWEEP PORT1\r\n");

    fprintf(PC, "Start CIGS data conversion\r\n");  
    unsigned int8 packetdata[PACKET_SIZE] = {0x00}; // Initialize packet data with 0x00
    

    
    //make_meas_header(packetdata, parameter); // Create measurement header
    unsigned int8 packetdata_index = 0; // Index for packet data
   
    for (unsigned int16 i = 0; i < measurement_step; i++)
    {        
        packetdata[packetdata_index]      = (cigs1_buffer[0][i] >> 4) & 0xFF;
        packetdata_index++; 
        packetdata[packetdata_index]    = (cigs1_buffer[0][i] & 0x0F) << 4 | (cigs1_buffer[1][i] >>8 )& 0x0F;
        packetdata_index++; 
        packetdata[packetdata_index]    = cigs1_buffer[1][i] & 0xFF;
        packetdata_index++; 

        if (packetdata_index == PACKET_SIZE-1) {
            // Add CRC
            //fprintf(PC, "Adding CRC to packetdata\r\n");
            packetdata[PACKET_SIZE-1] = calc_crc8(packetdata, PACKET_SIZE-1);            
            
            
            // Flash 書き込み処理
            unsigned int32 write_address = ADDRESS_MISF_MEASUREMENT_START + iv_data.used_counter;
            //fprintf(PC, "write data\r\n");
            write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
            //fprintf(PC, "Add Counter\r\n");
            // カウンタ更新
            //fprintf(PC, "before misf_meas_use_counter: %08LX\r\n", misf_meas_use_counter);
            iv_data.used_counter += PACKET_SIZE;
            //fprintf(PC, "after misf_meas_use_counter: %08LX\r\n", misf_meas_use_counter);
            iv_data.uncopied_counter += PACKET_SIZE;
            packetdata_index = 0; // Reset packet byte counter
            memset(packetdata, 0x00, PACKET_SIZE); // これ重要
            //fprintf(PC, "Added Counter\r\n");
        }
    }

    //fprintf(PC, "End CIGS data conversion\r\n");
    //fprintf(PC, "Start CIGS data conversion\r\n");
    
    if (packetdata_index < PACKET_SIZE-1) {
        // Fill the remaining bytes with 0x00
        for (unsigned int8 j = packetdata_index; j < PACKET_SIZE-1; j++ ) {
            packetdata[j] = 0x00;
        }
        // Flash 書き込み処理
        unsigned int32 write_address = ADDRESS_MISF_MEASUREMENT_START + iv_data.used_counter;
        write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
        //fprintf(PC, "before misf_meas_use_counter: %08LX\r\n", misf_meas_use_counter);
        // カウンタ更新
        iv_data.used_counter += PACKET_SIZE;
        //fprintf(PC, "after misf_meas_use_counter: %08LX\r\n", misf_meas_use_counter);
        iv_data.uncopied_counter += PACKET_SIZE;
    }
        
    write_misf_address_area();
    //misf_init(); // Initialize the mission flash
    fprintf(PC, "End CIGS data conversion\r\n");
    
}

void sweep_port2(unsigned int8 measurement_step)
{
    fprintf(PC, "Start SWEEP PORT2\r\n");
    fprintf(PC, "\tSweep step : %u\r\n", measurement_step);
    
    // Set CIGS2
    output_low(CONNECT_CIGS1);
    output_high(CONNECT_CIGS2);
    output_low(EN_NPWR); 

    MEASUREMENT_DATA measured_data;

    measured_data.time_sec = get_current_sec(); 
    measured_data.temp_py_top = ad7490_read(ADC_TEMP_PY_TOP);
    measured_data.temp_py_bot = ad7490_read(ADC_TEMP_PY_BOT);
    measured_data.temp_mis7 = ad7490_read(ADC_TEMP_MIS7);
    measured_data.pd = ad7490_read(ADC_PD); 

    unsigned int16 cigs2_buffer[2][0xFF]; // Buffer for CIGS2 data

    for (unsigned int8 count = 0; count < measurement_step; count++)
    {    
        // set DAC value
        mcp4901_2_write(count);
        //delay_ms(100); // wait for the DAC to stabilize

        // read CIGS voltage and current      
        delay_ms(1);
        cigs2_buffer[0][count] = ad7490_read(ADC_CIGS2_VOLT);
        cigs2_buffer[1][count] = ad7490_read(ADC_CIGS2_CURR);
    }

    output_low(CONNECT_CIGS2);
    output_high(EN_NPWR);

    fprintf(PC, "END SWEEP PORT2\r\n");

    fprintf(PC, "Start CIGS data conversion\r\n");  
}
/*
void add_smf_queue_data()
{
    SmfDataStruct smf_data;
    smf_data.type = SMF_DATA_TYPE_CIGS;
    smf_data.length = PACKET_SIZE;
    memcpy(smf_data.data, packetdata, PACKET_SIZE);
    enqueue_smf_data(&smf_data);
}
*/
void add_smf_queue_piclog()
{}

void convert_datas(MEASUREMENT_DATA measured_data, )
{

}

void sweep_with_threshold(unsigned int16 curr_threshold, unsigned int16 pd_threshold, unsigned int16 curr_limit)
{
    fprintf(PC, "Start SWEEP with threshold\r\n");
   
    // Enable both CIGS ports
    output_high(CONNECT_CIGS1);
    output_high(CONNECT_CIGS2);

    // delay_us(100); // wait for the CIGS to stabilize

    // Init Port1
    SWEEP_CONFIG port1 = {0};
    port1.port_num = 1;
    port1.sweep_step = 0;
    port1.active = 1;

    // Init Port2
    SWEEP_CONFIG port2 = {0};
    port2.port_num = 2;
    port2.sweep_step = 0;
    port2.active = 1;

    int16 count = 1;
    
    // Initialize DACs to 0
    mcp4901_1_write(1);
    mcp4901_2_write(1);
    ad7490_read(ADC_CIGS1_CURR);
    ad7490_read(ADC_CIGS2_CURR);
    ad7490_read(ADC_CIGS2_CURR);
    delay_ms(200);

    // Read initial PD value
    MEASUREMENT_DATA measured_data = create_meas_data();
    // Continue measurement while at least one port is active
    while (port1.active || port2.active)
    {
        // Set DAC values for both ports (synchronized timing)
        mcp4901_1_write(count);
        mcp4901_2_write(count);
        delay_ms(1); // wait for DAC to stabilize

        // Read CIGS1 data (port1) only if still active
        if (port1.active) {
            port1.data_buffer[0][count] = ad7490_read(ADC_CIGS1_AMP);
            port1.data_buffer[1][count] = ad7490_read(ADC_CIGS1_CURR);
            port1.sweep_step = count + 1; // Update CIGS1 step counter
        }
        
        // Read CIGS2 data (port2) only if still active
        if (port2.active) {
            port2.data_buffer[0][count] = ad7490_read(ADC_CIGS2_AMP);
            port2.data_buffer[1][count] = ad7490_read(ADC_CIGS2_CURR);
            fprintf(PC, "CIGS2 data: %04LX, %04LX\r\n", port2.data_buffer[0][count], port2.data_buffer[1][count]);
            port2.sweep_step = count + 1; // Update CIGS2 step counter
        }

        count++;

        // Check global exit conditions
        if (count >= 255) {
            fprintf(PC, "Maximum step count reached: %ld\r\n", count);
            break;
        }
        
        // Check CIGS1 specific conditions
        if (port1.active) {
            if (port1.data_buffer[1][count-1] < curr_limit) {
                fprintf(PC, "CIGS1 current limit reached:");
                port1.active = 0;
                output_low(CONNECT_CIGS1); // Disconnect CIGS1
            }
            else if (port1.data_buffer[1][count-1] < curr_threshold) {
                fprintf(PC, "CIGS1 current below threshold");
                port1.active = 0;
                output_low(CONNECT_CIGS1); // Disconnect CIGS1
            }
        }
        
        // Check CIGS2 specific conditions
        if (port2.active) {
            if (port2.data_buffer[1][count-1] < curr_limit) {
                fprintf(PC, "CIGS2 current limit reached: ");
                port2.active = 0;
                output_low(CONNECT_CIGS2); // Disconnect CIGS2
            }
            else if (port2.data_buffer[1][count-1] < curr_threshold) {
                fprintf(PC, "CIGS2 current below threshold:");
                port2.active = 0;
                output_low(CONNECT_CIGS2); // Disconnect CIGS2
            }
        }
    }

    // Ensure all connections are disabled
    output_low(CONNECT_CIGS1);
    output_low(CONNECT_CIGS2);
    // output_high(EN_NPWR);

    log_meas_data(&measured_data, &port1); // Log data for CIGS1
    log_meas_data(&measured_data, &port2); // Log data for CIGS2

    fprintf(PC, "End SWEEP with threshold\r\n");
}


void log_meas_data(MEASUREMENT_DATA *measured_data, SWEEP_CONFIG *port_data)
{
    unsigned int8 packetdata[PACKET_SIZE] = {0x00}; // Initialize packet data with 0x00
    unsigned int8 packetdata_index = 0; // Index for packet data


    // Header creation
    packetdata[0] = START_MAKER;
    packetdata[1] = (START_MAKER & 0xF0) | (port_data->port_num & 0x0F);
    packetdata[2] = measured_data->time_sec >> 24 & 0xFF;
    packetdata[3] = measured_data->time_sec >> 16 & 0xFF;
    packetdata[4] = measured_data->time_sec >> 8 & 0xFF;
    packetdata[5] = measured_data->time_sec & 0xFF;
    packetdata[6] = (measured_data->time_msec & 0x0F) >>4 | measured_data->pd >> 8 & 0x0F;
    packetdata[7] = measured_data->pd & 0xFF;
    packetdata[8] = (measured_data->open_voltage >> 4) & 0xFF; // PD start low byte
    packetdata[9] = (measured_data->open_voltage & 0x0F) << 4 | (measured_data->temp_py_top >> 8) & 0x0F;
    packetdata[10] = measured_data->temp_py_top & 0xFF;
    packetdata[11] = (measured_data->temp_py_bot >> 4) & 0xFF; // PD start low byte
    packetdata[12] = (measured_data->temp_py_bot & 0x0F) << 4 | (measured_data->temp_mis7 >> 8) & 0x0F;
    packetdata[13] = measured_data->temp_mis7 & 0xFF;

    packetdata_index = 14; // Start index for data

    for (unsigned int16 i = 0; i < port_data->sweep_step ; i++) {
        unsigned int16 data0 = port_data->data_buffer[0][i];
        unsigned int16 data1 = port_data->data_buffer[1][i];

        packetdata[packetdata_index++] = (data0 >> 4) & 0xFF;
        packetdata[packetdata_index++] = ((data0 & 0x0F) << 4) | ((data1 >> 8) & 0x0F);
        packetdata[packetdata_index++] = data1 & 0xFF;

        if (packetdata_index + 3 >= PACKET_SIZE - 1) {
            packetdata[PACKET_SIZE - 1] = calc_crc8(packetdata, PACKET_SIZE - 1);
            unsigned int32 write_address = ADDRESS_MISF_MEASUREMENT_START + iv_data.used_counter;
            write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
            iv_data.used_counter += PACKET_SIZE;
            iv_data.uncopied_counter += PACKET_SIZE;
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
        unsigned int32 write_address = ADDRESS_MISF_MEASUREMENT_START + iv_data.used_counter;
        write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
        iv_data.used_counter += PACKET_SIZE;
        iv_data.uncopied_counter += PACKET_SIZE;
    }

    fprintf(PC, "End CIGS data conversion\r\n");
    write_misf_address_area(); // Write the mission flash address area
}

MEASUREMENT_DATA create_meas_data()
{
    MEASUREMENT_DATA data;
    data.time_sec = get_current_sec();
    data.time_msec = get_current_msec();
    data.temp_py_top = ad7490_read(ADC_TEMP_PY_TOP);
    data.temp_py_bot = ad7490_read(ADC_TEMP_PY_BOT);
    data.temp_mis7 = ad7490_read(ADC_TEMP_MIS7);
    data.pd = ad7490_read(ADC_PD);
    data.open_voltage = ad7490_read(ADC_CIGS1_VOLT); // Assuming this is the correct ADC channel for open voltage
    return data;
}
