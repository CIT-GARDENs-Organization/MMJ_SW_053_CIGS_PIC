#include "../mmj_cigs_config.h"
#include "../mmj_cigs_flash.h"
#include "../../lib/mt25q.h"

void misf_init()
{
    fprintf(PC, "MISSION FLASH Initialize\r\n");
    output_high(MIS_FM_CS); // Set CS pin high to deselect the flash
    output_high(SMF_CS); // Set CS pin high to deselect the SMF
    FLASH_DATA_HEADER flash_data_header;

    if (!is_connect(mis_fm)) {
        fprintf(PC, "\tFlash is not connected\r\n");
        return;
    }else {
        fprintf(PC, "\tFlash is connected\r\n");
    }
    
    read_data_bytes(mis_fm, ADDRESS_MANEGE_START, flash_data_header.bytes, PACKET_SIZE); // Read the PICLOG data header
    fprintf(PC, "\tmisf_piclog_use_counter         : %lu\r\n", flash_data_header.fields.misf_piclog_use_counter);
    fprintf(PC, "\tmisf_piclog_uncopyed_counter    : %lu\r\n", flash_data_header.fields.misf_piclog_uncopyed_counter);
    fprintf(PC, "\tmisf_piclog_loop_counter        : %u\r\n", flash_data_header.fields.misf_piclog_loop_counter);   

    misf_piclog_use_counter = flash_data_header.fields.misf_meas_use_counter;
    misf_piclog_loop_counter = flash_data_header.fields.misf_piclog_loop_counter;
    misf_piclog_uncopyed_counter = flash_data_header.fields.misf_piclog_uncopyed_counter;
    
    fprintf(PC, "\tComplete\r\n");
}

void smf_init()
{
   //Flash smf = {SPI_1, MT25QL01GBBB, SPI1_CS};
}

void update_misf_address_area()
{
    FLASH_DATA_HEADER flash_data_header;
    flash_data_header.fields.misf_piclog_use_counter = misf_piclog_use_counter;
    flash_data_header.fields.misf_piclog_loop_counter = misf_piclog_loop_counter;
    flash_data_header.fields.misf_piclog_uncopyed_counter = misf_piclog_uncopyed_counter;
    write_data_bytes(mis_fm, ADDRESS_MANEGE_START, *flash_data_header.bytes, PACKET_SIZE);
    fprintf(PC, "Update MISSION FLASH Address Area\r\n");



}













