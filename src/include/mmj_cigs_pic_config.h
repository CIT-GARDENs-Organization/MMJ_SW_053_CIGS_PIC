#ifndef MMJ_CIGS_PIC_CONFIG_H
#define MMJ_CIGS_PIC_CONFIG_H

#include <16F1789.h>

#device ADC=12
#use delay(crystal=16MHz, clock=16MHz)
#fuses NOMCLR



//#define SELF_DEVICE_ID CIGS_PIC


//-------------Serial--------------------
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=BOSS)
#use rs232(baud=9600,parity=N,xmit=PIN_A3,bits=8,stream=PC,FORCE_SW)


// Flash Memory
#define DEBUG
#define USE_FLASH1G     //MT25QL01GBBB
#define USE_FLASH128M   //MT25QL128ABA

#define SMF_CS  PIN_C1
//#define SMF 
#use spi (MASTER, SPI1, BAUD=400000, MODE=0, BITS=8, STREAM=SMF)


#define PICF_CS PIN_C2
#use spi (MASTER, SPI1, BAUD=400000, MODE=0, BITS=8, STREAM=PICF)

// Flash Address
#define PICF_START_ADDR 0x00000000
#define PICF_END_ADDR   0x000FFFFF  
#define SMF_START_ADDR 0x00000000
#define SMF_END_ADDR   0x000FFFFF   



//-------------I/O port--------------------
// AI/O___ADC
#define TEMP_TOP PIN_A5
#define TEMP_BOT PIN_A3
#define CIGS_VOLT PIN_A0
#define CIGS_CURR PIN_A1
#define PD PIN_E0

// AI/O___DAC
#define Load PIN_A2
#define Load PIN_A6

// DI/O___




#endif // MMJ_CIGS_PIC_CONFIG_H


