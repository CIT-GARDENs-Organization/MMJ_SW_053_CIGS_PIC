#include "../flash.h"
#include "../../src/include/mmj_cigs_config.h"



void flash_init()
{
    //Flash mis_fm = {SPI_0, MT25QL128ABA, SPI0_CS};
    fprintf(PC, "Flash Initialize\r\n");
    unsigned int8 chip_id[20] = {0}; // Array to store chip ID
    
    output_low(SPI0_CS);                                                  //lower the CS PIN
    delay_ms(10);                                                   //wait for flash to be ready
    spi_xfer(MIS_FM_STREAM,0x9E);                                           //READ ID COMAND   (0x9F)
    fprintf(PC, "\t[MISF] <-  9F\r\n");                                            //print Chip ID
    for (unsigned int8 i = 0; i < 20; i++) {
        chip_id[i] = spi_xfer(MIS_FM_STREAM,0x00);                                  //read chip ID
        
    }
    for (unsigned int8 i = 0; i < 20; i++) {
        fprintf(PC, "%X ", chip_id[i]);                                         //print chip ID
    }
    //    fprintf(PC, "\r\n");
    output_high(SPI0_CS);                                                 //take CS PIN higher back       

    fprintf(PC, "\tComplete\r\n");
}
