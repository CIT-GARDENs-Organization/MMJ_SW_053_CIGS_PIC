#ifndef MMJ_CIGS_CONFIG_H
#define MMJ_CIGS_CONFIG_H

#include <16F1789.h>

#opt 0 // 0 = no optimization, 1 = optimize for speed, 2 = optimize for size

//#device ADC=12
#use delay(crystal=16MHz)
#fuses NOMCLR



//#define SELF_DEVICE_ID CIGS_PIC


//-------------Serial--------------------
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=BOSS)
#use rs232(baud=9600,parity=N,xmit=PIN_B4,bits=8,stream=PC,FORCE_SW)


// -------------SPI--------------------
// Shared Mission Flash
#define MIS_FM_CS  PIN_D1
#use spi(MASTER, SPI1, BAUD = 400000, MODE = 0, BITS= 8, STREAM = MIS_FM_STREAM)

// Mission Flash
#define SMF_CS PIN_D2
#use spi(MASTER, SPI1, BAUD = 400000, MODE = 0, BITS = 8, STREAM = SMF_STREAM)

// Analog to Digital Converter 
#define ADC_CS PIN_D3
#use spi(MASTER, SPI1, BAUD = 100000, MODE=2, BITS = 8, STREAM = ADC_STREAM)




/* DEMO
#use spi (MASTER, SPI1, ENABLE=PIN_A0, MODE=0, BITS=8, STREAM=MISF)
#use spi (MASTER, SPI1, ENABLE=PIN_A1, MODE=0, BITS=8, ENABLE_ACTIVE=1, STREAM=MISF)
#use spi (MASTER, SPI1, ENABLE=PIN_A3, BAUD=40000, MODE=2, BITS=16, ENABLE_ACTIVE=1, STREAM=ADC)
#use spi (MASTER, MODE=0, BITS=8, STREAM=SPI_4)
*/


//-------------I/O port--------------------

// AI/O___ADC
#define TEMP_TOP PIN_B0
#define TEMP_BOT PIN_A3
#define CIGS_VOLT PIN_A0 //PIN_A0
#define CIGS_CURR PIN_A1
#define PD PIN_E0
#define VREF PIN_A4

// AI/O___DAC
#define LOAD PIN_A5

// DI/O___
#define CONNECT_CIGS PIN_C0
#define EN_NPWR PIN_C3  

// DI/O RTC
#define RTC_PIN PIN_C1

// ADC Settings
#define TEMP_TOP_ADC AD7490_VIN0
#define TEMP_BOT_ADC AD7490_VIN1
#define CIGS_VOLT_ADC AD7490_VIN2
#define CIGS_CURR_ADC AD7490_VIN3
#define PD_ADC AD7490_VIN4

// Flasj Settings














#endif // MMJ_CIGS_CONFIG_H

//------------------End of File------------------
