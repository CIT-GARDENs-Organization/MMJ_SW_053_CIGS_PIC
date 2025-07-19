#include "../mmj_cigs_func.h"
#include "../mmj_cigs_config.h"
#include "../../lib/device/ad7490.h"
#include "../mmj_cigs_flash.h"
#include "../../lib/communication/mission_tools.h"
#include "../../lib/device/mcp4901.h"


//////////// Function to execute a mission based on the command content
void mcp4901_1_write(unsigned int16 value)
{
   unsigned int16 cmd = mcp4901_make_cmd(value);
   #ifdef MCP4901_DEBUG
      fprintf(PC, "\t[DAC] <<< %04LX\r\n", cmd);
   #endif
   output_low(DAC1_CS); // Select MCP4901
   delay_us(100); // Ensure CS is stable before sending data
   spi_xfer(ADCDAC_STREAM, cmd); // Send first byte
   output_high(DAC1_CS); // Deselect MCP4901
}

void mcp4901_2_write(unsigned int16 value)
{
   unsigned int16 cmd = mcp4901_make_cmd(value);
   #ifdef MCP4901_DEBUG
      fprintf(PC, "\t[DAC] <<< %04LX\r\n", cmd);
   #endif
   output_low(DAC2_CS); // Select MCP4901
   delay_us(100); // Ensure CS is stable before sending data
   spi_xfer(ADCDAC_STREAM, cmd); // Send first byte
   output_high(DAC2_CS); // Deselect MCP4901
}

unsigned int16 ad7490_read(int8 channel)
{   

    unsigned int16 cmd = ad7490_make_cmd(channel);
    
    // Send the command to the ADC
    output_low(ADC_CS); // Set CS pin low to select the ADC
    delay_us(10); // Wait for the ADC to stabilize
    spi_xfer(ADC_STREAM, cmd); // Dummy transfer to start communication
    #ifdef AD7490_DEBUG
       fprintf(PC,"\t[ADC] <<< %04LX\r\n", cmd);
    #endif
    delay_us(10); // Wait for the ADC to process the command
    output_high(ADC_CS); // Set CS pin high to end communication
    delay_us(10); // Wait for the ADC to process the command

    // Read the ADC data
    output_low(ADC_CS); 
    delay_us(10); // Wait for the ADC to stabilize
    unsigned int16 ans = spi_xfer(ADC_STREAM, 0x0000); // Read data from ADC
    output_high(ADC_CS); 
    unsigned int16 readdata = ans & 0x0FFF; //Conver LSB <--> MSB
    
    
    #ifdef AD7490_DEBUG
        fprintf(PC,"\t[ADC] >>> %04LX\r\n", readdata);
    #endif
    return readdata;
}

void io_init()
{
    fprintf(PC, "IO Initialize\r\n");
    output_low(CONNECT_CIGS1);
    output_high(EN_NPWR);
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
        cigs1_buffer[0][count] = ad7490_readdata(ADC_CIGS1_VOLT);
        cigs1_buffer[1][count] = ad7490_readdata(ADC_CIGS1_CURR);
        cigs2_buffer[0][count] = ad7490_readdata(ADC_CIGS2_VOLT);
        cigs2_buffer[1][count] = ad7490_readdata(ADC_CIGS2_CURR);
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
        delay_ms(1);
        cigs1_buffer[0][count] = ad7490_readdata(ADC_CIGS1_VOLT);
        cigs1_buffer[1][count] = ad7490_readdata(ADC_CIGS1_CURR);
    }

    output_low(CONNECT_CIGS1);
    output_high(EN_NPWR);

    fprintf(PC, "END SWEEP PORT1\r\n");

    fprintf(PC, "Start CIGS data conversion\r\n");  
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
        cigs2_buffer[0][count] = ad7490_readdata(ADC_CIGS2_VOLT);
        cigs2_buffer[1][count] = ad7490_readdata(ADC_CIGS2_CURR);
    }

    output_low(CONNECT_CIGS2);
    output_high(EN_NPWR);

    fprintf(PC, "END SWEEP PORT2\r\n");

    fprintf(PC, "Start CIGS data conversion\r\n");  
}