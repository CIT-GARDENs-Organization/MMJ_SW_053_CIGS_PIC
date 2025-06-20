#include "../mmj_cigs_config.h"
#include "../mmj_cigs_flash.h"
#include "../../lib/mt25q.h"

void misf_init()
{
    fprintf(PC, "MISSION FLASH Initialize\r\n");
    output_high(MIS_FM_CS); // Set CS pin high to deselect the flash
    output_high(SMF_CS); // Set CS pin high to deselect the SMF
    
    FLASH_DATA_HEADER flash_data_header;
    unsigned int8 readdata[PACKET_SIZE];

    if (!is_connect(mis_fm)) {
        fprintf(PC, "\tFlash is not connected\r\n");
        return;
    }else {
        fprintf(PC, "\tFlash is connected\r\n");
    }
    read_data_bytes(mis_fm, ADDRESS_MANEGE_START, readdata, PACKET_SIZE); // Read the PICLOG data header
    
    // Update the flash data header with the read data
    smf_piclog_use_counter = readdata[0] | (readdata[1] << 8) | (readdata[2] << 16) | (readdata[3] << 24);
    smf_piclog_loop_counter = readdata[4];
    smf_meas_use_counter = readdata[5] | (readdata[6] << 8) | (readdata[7] << 16) | (readdata[8] << 24);
    smf_meas_loop_counter = readdata[9];
    
    misf_piclog_use_counter = readdata[10] | (readdata[11] << 8) | (readdata[12] << 16) | (readdata[13] << 24);
    misf_piclog_loop_counter = readdata[14];
    misf_piclog_uncopyed_counter = readdata[15] | (readdata[16] << 8) | (readdata[17] << 16) | (readdata[18] << 24);
    misf_meas_use_counter = readdata[19] | (readdata[20] << 8) | (readdata[21] << 16) | (readdata[22] << 24);
    misf_meas_loop_counter = readdata[23];
    misf_meas_uncopyed_counter = readdata[24] | (readdata[25] << 8) | (readdata[26] << 16) | (readdata[27] << 24);  
    
    fprintf(PC, "\tSMF PICLOG Use Counter: %lu\r\n", smf_piclog_use_counter);
    fprintf(PC, "\tSMF PICLOG Loop Counter: %u\r\n", smf_piclog_loop_counter);
    fprintf(PC, "\tSMF MEAS Use Counter: %lu\r\n", smf_meas_use_counter);
    fprintf(PC, "\tSMF MEAS Loop Counter: %u\r\n", smf_meas_loop_counter);
    fprintf(PC, "\tMISF PICLOG Use Counter: %lu\r\n", misf_piclog_use_counter);
    fprintf(PC, "\tMISF PICLOG Loop Counter: %u\r\n", misf_piclog_loop_counter);
    fprintf(PC, "\tMISF PICLOG Uncopyed Counter: %lu\r\n", misf_piclog_uncopyed_counter);
    fprintf(PC, "\tMISF MEAS Use Counter: %lu\r\n", misf_meas_use_counter);
    fprintf(PC, "\tMISF MEAS Loop Counter: %u\r\n", misf_meas_loop_counter);
    fprintf(PC, "\tMISF MEAS Uncopyed Counter: %lu\r\n", misf_meas_uncopyed_counter);  
    
    fprintf(PC, "Complete\r\n");
}

void smf_init()
{
   //Flash smf = {SPI_1, MT25QL01GBBB, SPI1_CS};
}

void update_misf_address_area()
{
    fprintf(PC, "Update MISF Address Area\r\n");
    FLASH_DATA_HEADER flash_data_header;
    unsigned int8 readdata[PACKET_SIZE];
    read_data_bytes(mis_fm, ADDRESS_MANEGE_START, readdata, PACKET_SIZE); // Read the PICLOG data header
    // Update the flash data header with the read data
    flash_data_header.smf_piclog_use_counter = readdata[0] | (readdata[1] << 8) | (readdata[2] << 16) | (readdata[3] << 24);
    flash_data_header.smf_piclog_loop_counter = readdata[4];
    flash_data_header.smf_meas_use_counter = readdata[5] | (readdata[6] << 8) | (readdata[7] << 16) | (readdata[8] << 24);
    flash_data_header.smf_meas_loop_counter = readdata[9];
    flash_data_header.misf_piclog_use_counter = readdata[10] | (readdata[11] << 8) | (readdata[12] << 16) | (readdata[13] << 24);
    flash_data_header.misf_piclog_loop_counter = readdata[14];
    flash_data_header.misf_piclog_uncopyed_counter = readdata[15] | (readdata[16] << 8) | (readdata[17] << 16) | (readdata[18] << 24);
    flash_data_header.misf_meas_use_counter = readdata[19] | (readdata[20] << 8) | (readdata[21] << 16) | (readdata[22] << 24);
    flash_data_header.misf_meas_loop_counter = readdata[23];
    flash_data_header.misf_meas_uncopyed_counter = readdata[24] | (readdata[25] << 8) | (readdata[26] << 16) | (readdata[27] << 24);
    // Write the updated flash data header back to the flash memory
}













