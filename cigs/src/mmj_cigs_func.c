#include "../mmj_cigs_func.h"
#include "../mmj_cigs_config.h"
#include "../mmj_cigs_piclog.h"
#include "../../lib/ad7490.h"
#include "../mmj_cigs_flash.h"
#include "../../lib/mission_tools.h"

/*
struct AD7490_STREAM ad7490_stream = {
    ADC_STREAM,    // spi_stream
    ADC_CS  // cs_pin
};
*/

////


void io_init()
{
    fprintf(PC, "IO Initialize\r\n");
    output_low(CONNECT_CIGS);
    output_high(EN_NPWR);
    fprintf(PC, "\tComplete\r\n");
}

void adc_init()
{
   fprintf(PC, "ADC Initialize\r\n");
    //setup ADC
   setup_oscillator(OSC_16MHZ);   //oscillator speed (crystal)
   //setup_adc_ports(CIGS_VOLT | CIGS_CURR | TEMP_BOT | TEMP_TOP | PD , DAC_VSS_FVR);
   setup_adc(ADC_CLOCK_DIV_64);

   //setup DAC
   setup_dac(DAC_OUTPUT1 | DAC_VSS_VDD);                                        //DAC output at pin DAC output1 from 0-VDD volts  
   dac_write(0);                                                               
   fprintf(PC, "\tComplete\r\n");
}

#Separate
void sweep(unsigned int8 parameter[])
{
    // 
    fprintf(PC, "Start SWEEP\r\n");
    unsigned int8 measurement_step = parameter[1]; // Get the measurement step from the parameter array
    fprintf(PC, "\tSweep step : %u\r\n", measurement_step);
    output_high(CONNECT_CIGS);
    delay_us(100); // wait for the CIGS to stabilize

    // read header data
    /*
    measured_time = get_current_sec(); // read time from timer0
    measured_pd = ad7490_read(ADC_PD); // read PD value
    measured_temp_top = ad7490_read(ADC_TEMP_TOP); // read temperature at top
    measured_temp_bot = ad7490_read(ADC_TEMP_BOT); // read temperature at bottom
    measured_open_voltage = ad7490_read(ADC_CIGS_VOLT); // read open voltage at CIGS
    */
   
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

    output_low(CONNECT_CIGS);
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

