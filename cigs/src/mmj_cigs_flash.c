#include "../mmj_cigs_config.h"
#include "../mmj_cigs_flash.h"
#include "../../lib/mt25q.h"

void misf_init()
{
    fprintf(PC, "MISSION FLASH Initialize\r\n");
    output_high(MIS_FM_CS); // Set CS pin high to deselect the flash
    output_high(SMF_CS); // Set CS pin high to deselect the SMF
    PICLOG_DATA_HEADER piclog_data_header;
    MEAS_DATA_HEADER meas_data_header;

    if (!is_connect(mis_fm)) {
        fprintf(PC, "Flash is not connected\r\n");
        return;
    }
    read_data_bytes(mis_fm, ADDRESS_PICLOG_START, piclog_data_header.bytes, sizeof(piclog_data_header.bytes)); // Clear the stream
    fprintf(PC, "\t__PICLOG__\r\n");
    fprintf(PC, "\t\tpiclog_write_counter   : %lu\r\n", piclog_data_header.fields.piclog_write_counter);
    fprintf(PC, "\t\tpiclog_loop_counter    : %d\r\n", piclog_data_header.fields.piclog_loop_counter);
    fprintf(PC, "\t\tpiclog_smfwrite_counter: %lu\r\n", piclog_data_header.fields.piclog_smfwrite_counter);
    fprintf(PC, "\t\tpiclog_smf_loop_counter: %d\r\n", piclog_data_header.fields.piclog_smf_loop_counter);

    read_data_bytes(mis_fm, ADDRESS_PICLOG_START, meas_data_header.bytes, sizeof(meas_data_header.bytes)); // Clear the stream
    fprintf(PC, "\t__MEASUREMENT__\r\n");
    fprintf(PC, "\t\tmeasurement_data_counter    : %lu\r\n", meas_data_header.fields.measurement_data_counter); 
    fprintf(PC, "\t\tmeas_loop_counter           : %d\r\n", meas_data_header.fields.meas_loop_counter);
    fprintf(PC, "\t\tmeasurement_data_smf_counter: %lu\r\n", meas_data_header.fields.measurement_data_smf_counter);
    fprintf(PC, "\t\tmeas_smf_loop_counter       : %d\r\n", meas_data_header.fields.meas_smf_loop_counter);
    fprintf(PC, "\tComplete\r\n");
}

void smf_init()
{
   //Flash smf = {SPI_1, MT25QL01GBBB, SPI1_CS};
}















