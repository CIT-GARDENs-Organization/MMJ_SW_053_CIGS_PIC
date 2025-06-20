#include "../mmj_cigs_piclog.h"
#include "../../lib/timer.h"
#include "../mmj_cigs_flash.h"

#define PICLOG_DEBUG

void piclog_make(unsigned int8 function, unsigned int8 parameter)
{
    unsigned int8 piclog[PICLOG_PACKET_SIZE];
    int32 time = get_current_sec();
    piclog[0] = (time >> 24) & 0xFF; // Time high byte
    piclog[1] = (time >> 16) & 0xFF;
    piclog[2] = (time >> 8) & 0xFF;  // Time middle byte
    piclog[3] = time & 0xFF;         // Time low byte
    piclog[4] = function;            // Function code
    piclog[5] = parameter;           // Parameter code

    #ifdef PICLOG_DEBUG
        fprintf(PC, "[PICLOG] : ");
        for (unsigned int8 i = 0; i < PICLOG_PACKET_SIZE; i++) {
            fprintf(PC, "%02X ", piclog[i]);
        }
        fprintf(PC, "\r\n");
    #endif

    unsigned int32 write_address = ADDRESS_MISF_PICLOG_DATA_START + misf_piclog_use_counter;
    //fprintf(PC, "Write PICLOG to address: 0x%08LX\r\n", write_address);
    
    if(is_connect(mis_fm) == FALSE) {
        fprintf(PC, "Mission Flash is not connected\r\n");
        return;
    }
    
    write_data_bytes(mis_fm, write_address, piclog, PICLOG_PACKET_SIZE);
    
    // Update the counters
    misf_piclog_use_counter += PICLOG_PACKET_SIZE;
    misf_piclog_uncopyed_counter += PICLOG_PACKET_SIZE;
    misf_piclog_write_counter += PICLOG_PACKET_SIZE;

    // Next Packet
    if (misf_piclog_write_counter + PICLOG_PACKET_SIZE >=  PACKET_SIZE) {
        write_address = ADDRESS_MISF_PICLOG_DATA_START + misf_piclog_use_counter;
        write_data_bytes(mis_fm, write_address, PICLOG_BLANK_DATA, PICLOG_PACKET_SIZE);
        misf_piclog_write_counter = 0;
    }


    write_misf_address_area(); // Update the address area after writing
    
    /*
    // Add CRC Check
    if (misf_piclog_write_counter >= MISF_PICLOG_MAX_COUNT) {
        write_address = ADDRESS_MISF_PICLOG_DATA_START + misf_piclog_use_counter;
        write_data_bytes(mis_fm, write_address, *PICLOG_BLANK_DATA, PICLOG_BLANK_SIZE);
        unsigned int8 piclog_data_header[PICLOG_BLANK_SIZE] = {0x00, 0x00, 0x00}; // Initialize blank data
        misf_piclog_write_counter = 0; // Reset if max count reached
    }
    */
    #ifdef PICLOG_DEBUG
        //fprintf(PC, "PICLOG written: %02X %02X %02X %02X %02X %02X\r\n", piclog_data.bytes[0], data[1], data[2], data[3], data[4], data[5]);
    #endif
}