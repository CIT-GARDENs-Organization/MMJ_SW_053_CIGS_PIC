#include "../lib/flash.h"
#include "include/mmj_cigs_config.h"



void misf_init()
{
    //Flash mis_fm = {SPI_0, MT25QL128ABA, SPI0_CS};
    fprintf(PC, "Flash Initialize\r\n");
    misf_read_id(); // Read the chip ID
    
    fprintf(PC, "\r\n\tComplete\r\n");
}

void misf_read_id()
{
    unsigned int8 ans[READ_ID_DATA_SIZE] = {0}; // Array to store chip ID
    output_low(SPI0_CS);                                                  //lower the CS PIN
    delay_ms(10);                                                   //wait for flash to be ready
    spi_xfer(MIS_FM_STREAM,0x9E);                                           //READ ID COMAND   (0x9F)                                           //print Chip ID
    for (unsigned int8 i = 0; i < READ_ID_DATA_SIZE; i++) {
        ans[i] = spi_xfer(MIS_FM_STREAM,0x00);                                  //read chip ID
        
    }
    output_high(SPI0_CS); 

    if ( ans[0] == MANUFACTURER_ID && ans[1] == MEMORY_TYPE ){
        if (ans[2] == MEMORY_CAPACITY_128M ) {
            fprintf(PC, "\t[MISF] Connect success.\r\n");
        } else if (ans[2] == MEMORY_CAPACITY_1G) {
                fprintf(PC, "\t[MISF] <- 9F\r\n"); 
            fprintf(PC, "\t[MISF] -> ");
            for (unsigned int8 i = 0; i < READ_ID_DATA_SIZE; i++) {
                fprintf(PC, "%02X ", ans[i]);                                         //print chip ID
            }
            fprintf(PC, "\r\n");
            fprintf(PC, "\t[MISF] MEMORY_CAPACITY error! This is SMF\r\n");

        } else{
            fprintf(PC, "\t[MISF] <- 9F\r\n"); 
            fprintf(PC, "\t[MISF] -> ");
            for (unsigned int8 i = 0; i < READ_ID_DATA_SIZE; i++) {
                fprintf(PC, "%02X ", ans[i]);                                         //print chip ID
            }
            fprintf(PC, "\r\n\t[MISF] MEMORY_CAPACITY error! Chip ID is invalid\r\n");
        }
    } else {
            fprintf(PC, "\t[MISF] <- 9F\r\n"); 
            fprintf(PC, "\t[MISF] -> ");
            for (unsigned int8 i = 0; i < READ_ID_DATA_SIZE; i++) {
                fprintf(PC, "%02X ", ans[i]);                                         //print chip ID
            }
            fprintf(PC, "\r\n\t[MISF] Chip ID is invalid\r\n");
    } 
}
