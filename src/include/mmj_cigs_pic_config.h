#ifndef MMJ_CIGS_PIC_CONFIG_H
#define MMJ_CIGS_PIC_CONFIG_H

#include <16F1789.h>

#device ADC=12
#use delay(crystal=16MHz, clock=16MHz)
#fuses NOMCLR



//#define SELF_DEVICE_ID CIGS_PIC


//-------------Serial--------------------
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=BOSS)
#use rs232(baud=9600,parity=N,xmit=PIN_B4,bits=8,stream=PC,FORCE_SW)


#define SPI0_CS  PIN_C1
#use spi(MASTER, SPI1, BAUD=400000, MODE=0, BITS=8, STREAM=MIS_FM_STREAM)

#define SPI1_CS PIN_C2
#use spi(MASTER, SPI1, BAUD=400000, MODE=0, BITS=8, STREAM=SMF_STREAM)



//-------------I/O port--------------------
// AI/O___ADC
#define TEMP_TOP PIN_A5
#define TEMP_BOT PIN_A3
#define CIGS_VOLT PIN_A0 //PIN_A0
#define CIGS_CURR PIN_A1
#define PD PIN_E0

// AI/O___DAC
#define LOAD PIN_A2

// DI/O___
#define CIGS_CONNECT PIN_C0
#define EN_NPWR PIN_C3  //Negative Power


#endif // MMJ_CIGS_PIC_CONFIG_H

//------------------End of File------------------
