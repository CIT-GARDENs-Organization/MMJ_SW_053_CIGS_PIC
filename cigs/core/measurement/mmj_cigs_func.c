#include "mmj_cigs_func.h"               // 同じフォルダのヘッダー
#include "../../system/mmj_cigs_config.h"      // システム設定
#include "../../hardware/devices/ad7490_driver.h"   // デバイス定義  
#include "../../hardware/devices/mcp4901_driver.h"
#include "../storage/mmj_cigs_flash.h"               // ストレージ機能
#include "../../../lib/communication/mission_tools.h"   // 通信ツール

void io_init()
{
    fprintf(PC, "IO Initialize\r\n");
    output_high(ADC_CS); // ADC Chip Select
    output_high(MIS_FM_CS); // DAC Chip Select
    output_high(SMF_CS); // SMF Chip Select
    output_low(CONNECT_CIGS1);
    output_high(EN_NPWR);
    delay_ms(1);
    fprintf(PC, "\tComplete\r\n");
}

void adc_init()
{
   fprintf(PC, "ADC Initialize\r\n");
    //setup ADC
   /*
   setup_oscillator(OSC_16MHZ);   //oscillator speed (crystal)
   //setup_adc_ports(CIGS_VOLT | CIGS_CURR | TEMP_BOT | TEMP_TOP | PD , DAC_VSS_FVR);
   setup_adc(ADC_CLOCK_DIV_64);

   //setup DAC
   setup_dac(DAC_OUTPUT1 | DAC_VSS_VDD);                                        //DAC output at pin DAC output1 from 0-VDD volts  
   dac_write(0);                                                               
   */
   fprintf(PC, "\tComplete\r\n");
}



void sweep(unsigned int8 parameter[])
{
    // 
    fprintf(PC, "Start SWEEP\r\n");
    unsigned int8 measurement_step = parameter[1]; // Get the measurement step from the parameter array
    fprintf(PC, "\tSweep step : %u\r\n", measurement_step);
    output_high(CONNECT_CIGS1);
    delay_us(100); // wait for the CIGS to stabilize
      /*
    MEASUREMENT_DATA measured_data;

    measured_data.time = get_current_sec(); 
    measured_data.temp_py_top = ad7490_read(ADC_TEMP_PY_TOP);
    measured_data.temp_py_bot = ad7490_read(ADC_TEMP_PY_BOT);
    measured_data.temp_mis7 = ad7490_read(ADC_TEMP_MIS7);
    measured_data.pd = ad7490_read(ADC_PD); 

    for (unsigned int8 count = 0; count < measurement_step; count++)
    {    
        // set DAC value
        dac_write(count);
        //delay_ms(100); // wait for the DAC to stabilize

        // read CIGS voltage and current      
        delay_ms(1); // wait for the ADC to stabilize
        data_buffer[count*2] = ad7490_readdata(0x8330);//0xAF30 Jumper 
        data_buffer[count*2+1] = ad7490_readdata(0x8730); // read voltage at adc pin
        
        // monitoring
        fprintf(PC, "%04LX,%04LX\r\n", data_buffer[count*2], data_buffer[count*2+1]);
    }

    output_low(CONNECT_CIGS1);
    fprintf(PC, "END SWEEP\r\n");

    fprintf(PC, "Start CIGS data conversion\r\n");
    unsigned int8 packetdata[PACKET_SIZE] = {0x00}; // Initialize packet data with 0x00
    

    
    //make_meas_header(packetdata, parameter); // Create measurement header
    unsigned int8 packetdata_index = 0; // Index for packet data
   
    for (unsigned int16 i = 0; i < measurement_step; i++)
    {        
        packetdata[packetdata_index]      = (data_buffer[i*2] >> 4) & 0xFF;
        packetdata_index++; 
        packetdata[packetdata_index]    = (data_buffer[i*2] & 0x0F) << 4 | (data_buffer[i*2+1] >>8 )& 0x0F;
        packetdata_index++; 
        packetdata[packetdata_index]    = data_buffer[i*2+1] & 0xFF;
        packetdata_index++; 

        if (packetdata_index == PACKET_SIZE-1) {
            // Add CRC
            //fprintf(PC, "Adding CRC to packetdata\r\n");
            packetdata[PACKET_SIZE-1] = calc_crc8(packetdata, PACKET_SIZE-1);            
            
            
            // Flash 書き込み処理
            unsigned int32 write_address = ADDRESS_MISF_MEASUREMENT_START + misf_meas_use_counter;
            //fprintf(PC, "write data\r\n");
            write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
            //fprintf(PC, "Add Counter\r\n");
            // カウンタ更新
            //fprintf(PC, "before misf_meas_use_counter: %08LX\r\n", misf_meas_use_counter);
            misf_meas_use_counter += PACKET_SIZE;
            //fprintf(PC, "after misf_meas_use_counter: %08LX\r\n", misf_meas_use_counter);
            misf_meas_uncopyed_counter += PACKET_SIZE;
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
        unsigned int32 write_address = ADDRESS_MISF_MEASUREMENT_START + misf_meas_use_counter;
        write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
        //fprintf(PC, "before misf_meas_use_counter: %08LX\r\n", misf_meas_use_counter);
        // カウンタ更新
        misf_meas_use_counter += PACKET_SIZE;
        //fprintf(PC, "after misf_meas_use_counter: %08LX\r\n", misf_meas_use_counter);
        misf_meas_uncopyed_counter += PACKET_SIZE;
    }
        
    write_misf_address_area();
    //misf_init(); // Initialize the mission flash
    fprintf(PC, "End CIGS data conversion\r\n");
    */
}

void make_meas_header(unsigned int8 *packetdata, unsigned int8 *cmd)
{
    packetdata[0] = START_MAKER; // Header start
    packetdata[1] = START_MAKER; // Header start
    
    // Add uplink cmd
    packetdata[2]  = cmd[0]; // Command ID
    packetdata[3]  = cmd[1]; // Param1
    packetdata[4]  = cmd[2]; // Param2
    packetdata[5]  = cmd[3]; // Param3 
    packetdata[6]  = cmd[4]; // Param4
    packetdata[7]  = cmd[5]; // Param5
    packetdata[8]  = cmd[6]; // Param6
    packetdata[9]  = cmd[7]; // Param7
    packetdata[10] = cmd[8]; // Reserved byte

    // Add timestamp
    packetdata[11] = measured_time >> 24 & 0xFF; // Time high byte
    packetdata[12] = measured_time >> 16 & 0xFF; //
    packetdata[13] = measured_time >> 8 & 0xFF;  // Time middle byte
    packetdata[14] = measured_time & 0xFF;         // Time low
    
    
    //cigs_data[i*3+1]    = (data_buffer[i*2] & 0x0F) << 4 | (data_buffer[i*2+1] >>8 )& 0x0F;

    // Add PD value
    packetdata[15] = ( measured_pd >> 4 ) &  0xFF; // PD start low byte
    packetdata[16] = ( measured_pd & 0x0F ) << 4 | ( measured_open_voltage >>8 )& 0x0F;
    packetdata[17] = measured_open_voltage & 0xFF; // PD end
    packetdata[18] = ( measured_temp_top >> 4 ) &  0xFF; // PD start low byte
    packetdata[19] = ( measured_temp_top & 0x0F ) << 4 | ( measured_temp_bot >>8 )& 0x0F;
    packetdata[20] = measured_temp_bot & 0xFF; // PD end
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

    measured_data.time = get_current_sec(); 
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

    measured_data.time = get_current_sec(); 
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
            unsigned int32 write_address = ADDRESS_MISF_MEASUREMENT_START + misf_meas_use_counter;
            //fprintf(PC, "write data\r\n");
            write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
            //fprintf(PC, "Add Counter\r\n");
            // カウンタ更新
            //fprintf(PC, "before misf_meas_use_counter: %08LX\r\n", misf_meas_use_counter);
            misf_meas_use_counter += PACKET_SIZE;
            //fprintf(PC, "after misf_meas_use_counter: %08LX\r\n", misf_meas_use_counter);
            misf_meas_uncopyed_counter += PACKET_SIZE;
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
        unsigned int32 write_address = ADDRESS_MISF_MEASUREMENT_START + misf_meas_use_counter;
        write_data_bytes(mis_fm, write_address, packetdata, PACKET_SIZE);
        //fprintf(PC, "before misf_meas_use_counter: %08LX\r\n", misf_meas_use_counter);
        // カウンタ更新
        misf_meas_use_counter += PACKET_SIZE;
        //fprintf(PC, "after misf_meas_use_counter: %08LX\r\n", misf_meas_use_counter);
        misf_meas_uncopyed_counter += PACKET_SIZE;
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

    measured_data.time = get_current_sec(); 
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
    fprintf(PC, "\tCurrent threshold: %u\r\n", curr_threshold);
    fprintf(PC, "\tPD threshold: %u\r\n", pd_threshold);
    fprintf(PC, "\tCurrent limit: %u\r\n", curr_limit);
    
    // Enable both CIGS ports
    output_high(CONNECT_CIGS1);
    output_high(CONNECT_CIGS2);
    output_low(EN_NPWR);
    delay_us(100); // wait for the CIGS to stabilize

    int16 pd_value;
    int16 count = 0;
    int16 cigs1_buffer[2][255]; // Buffer for CIGS1 voltage and current data
    int16 cigs2_buffer[2][255]; // Buffer for CIGS2 voltage and current data
    
    // Port status flags
    int1 cigs1_active = 1; // CIGS1 is active
    int1 cigs2_active = 1; // CIGS2 is active
    
    // Individual step counters for each port
    int16 cigs1_steps = 0; // Number of steps measured for CIGS1
    int16 cigs2_steps = 0; // Number of steps measured for CIGS2

    // Initialize DACs to 0
    mcp4901_1_write(0);
    mcp4901_2_write(0);
    delay_ms(10);

    // Read initial PD value
    pd_value = ad7490_read(ADC_PD);
    fprintf(PC, "Initial PD value: %d\r\n", pd_value);

    // Continue measurement while at least one port is active
    while (cigs1_active || cigs2_active)
    {
        // Set DAC values for both ports (synchronized timing)
        mcp4901_1_write(count);
        mcp4901_2_write(count);
        delay_ms(1); // wait for DAC to stabilize

        // Read CIGS1 data (port1) only if still active
        if (cigs1_active) {
            cigs1_buffer[0][count] = ad7490_read(ADC_CIGS1_VOLT);
            cigs1_buffer[1][count] = ad7490_read(ADC_CIGS1_CURR);
            cigs1_steps = count + 1; // Update CIGS1 step counter
        }
        
        // Read CIGS2 data (port2) only if still active
        if (cigs2_active) {
            cigs2_buffer[0][count] = ad7490_read(ADC_CIGS2_VOLT);
            cigs2_buffer[1][count] = ad7490_read(ADC_CIGS2_CURR);
            cigs2_steps = count + 1; // Update CIGS2 step counter
        }

        // Monitor progress
        if (count % 10 == 0) {
            fprintf(PC, "Step %d - CIGS1: V=%d, I=%d (active=%d) - CIGS2: V=%d, I=%d (active=%d)\r\n", 
                    count, 
                    cigs1_active ? cigs1_buffer[0][count] : 0, cigs1_active ? cigs1_buffer[1][count] : 0, cigs1_active,
                    cigs2_active ? cigs2_buffer[0][count] : 0, cigs2_active ? cigs2_buffer[1][count] : 0, cigs2_active);
        }

        count++;

        // Check global exit conditions
        if (count >= 255) {
            fprintf(PC, "Maximum step count reached: %d\r\n", count);
            break;
        }
        
        // Check PD threshold (global condition)
        pd_value = ad7490_read(ADC_PD);
        if (pd_value < pd_threshold) {
            fprintf(PC, "PD value below threshold: %d < %u - stopping all measurements\r\n", pd_value, pd_threshold);
            break;
        }
        
        // Check CIGS1 specific conditions
        if (cigs1_active) {
            if (cigs1_buffer[1][count-1] > curr_limit) {
                fprintf(PC, "CIGS1 current limit reached: %d > %u - stopping CIGS1 at step %d\r\n", cigs1_buffer[1][count-1], curr_limit, cigs1_steps);
                cigs1_active = 0;
                output_low(CONNECT_CIGS1); // Disconnect CIGS1
            }
            else if (cigs1_buffer[1][count-1] < curr_threshold) {
                fprintf(PC, "CIGS1 current below threshold: %d < %u - stopping CIGS1 at step %d\r\n", cigs1_buffer[1][count-1], curr_threshold, cigs1_steps);
                cigs1_active = 0;
                output_low(CONNECT_CIGS1); // Disconnect CIGS1
            }
        }
        
        // Check CIGS2 specific conditions
        if (cigs2_active) {
            if (cigs2_buffer[1][count-1] > curr_limit) {
                fprintf(PC, "CIGS2 current limit reached: %d > %u - stopping CIGS2 at step %d\r\n", cigs2_buffer[1][count-1], curr_limit, cigs2_steps);
                cigs2_active = 0;
                output_low(CONNECT_CIGS2); // Disconnect CIGS2
            }
            else if (cigs2_buffer[1][count-1] < curr_threshold) {
                fprintf(PC, "CIGS2 current below threshold: %d < %u - stopping CIGS2 at step %d\r\n", cigs2_buffer[1][count-1], curr_threshold, cigs2_steps);
                cigs2_active = 0;
                output_low(CONNECT_CIGS2); // Disconnect CIGS2
            }
        }
    }

    // Ensure all connections are disabled
    output_low(CONNECT_CIGS1);
    output_low(CONNECT_CIGS2);
    output_high(EN_NPWR);
    
    fprintf(PC, "END SWEEP with threshold - Total steps: %d\r\n", count);
    fprintf(PC, "CIGS1 measured steps: %d\r\n", cigs1_steps);
    fprintf(PC, "CIGS2 measured steps: %d\r\n", cigs2_steps);
    
    // Data conversion and storage can be added here similar to other sweep functions
    fprintf(PC, "Start CIGS data conversion\r\n");
    // TODO: Implement data storage to flash memory if needed
    fprintf(PC, "End CIGS data conversion\r\n");
}

void log_meas_data(MEASUREMENT_DATA *measured_data, unsigned int8 port_num, int8 *data_buffer)
{
    
}