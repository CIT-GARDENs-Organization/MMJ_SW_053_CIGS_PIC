#ifndef MMJ_CIGS_CONFIG_H
#define MMJ_CIGS_CONFIG_H


#define SELF_DEVICE_ID CIGS_PIC

#include <18F67J94.h>
//#include <16F1789.h>
#opt 0 // 0 = no optimization, 1 = optimize for speed, 2 = optimize for size

#device ADC=12
#use delay(crystal=16MHz)
#fuses HS



#define PIC18
//#define PIC16



#ifdef PIC16
    //#include <16F1789.h>
    #fuses NOMCLR
    //-------------Serial--------------------
    #use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=BOSS)
    #use rs232(baud=9600,parity=N,xmit=PIN_B4,bits=8,stream=PC,FORCE_SW)


    // -------------SPI--------------------
    // MOSI:PIN_C5, MISO:PIN_C4, SCK:PIN_C3
    // Shared Mission Flash
    #define MIS_FM_CS  PIN_D3
    #use spi(MASTER, SPI1, BAUD = 100000, MODE = 0, BITS= 8, STREAM = MIS_FM_STREAM)

    // Mission Flash
    #define SMF_CS PIN_D1
    #use spi(MASTER, SPI1, BAUD = 100000, MODE = 0, BITS = 8, STREAM = SMF_STREAM)
    #use spi(MASTER, SPI1, BAUD = 100000, MODE = 0, BITS = 8, STREAM = spi_cell_write)
    // Analog to Digital Converter 
    #define ADC_CS PIN_D2
    //#use spi(MASTER, SPI1, BAUD = 9600, MODE=2, BITS = 16, STREAM = ADC_STREAM)
    #use spi(MASTER, CLK = PIN_C3, DI = PIN_C4, DO = PIN_C5, ENABLE = PIN_D2, BAUD = 57600, BITS = 16, STREAM = ADC_STREAM, MODE = 2 )
    
    
    // AI/O___ADC
    #define TEMP_TOP PIN_B0
    #define TEMP_BOT PIN_A3
    #define CIGS_VOLT PIN_A0 //PIN_A0
    #define CIGS_CURR PIN_A1
    #define PD PIN_E0
    #define VREF PIN_A4
 
    // AI/O___DAC
    #define LOAD PIN_A5

    /* DEMO
    #use spi (MASTER, SPI1, ENABLE=PIN_A0, MODE=0, BITS=8, STREAM=MISF)
    #use spi (MASTER, SPI1, ENABLE=PIN_A1, MODE=0, BITS=8, ENABLE_ACTIVE=1, STREAM=MISF)
    #use spi (MASTER, SPI1, ENABLE=PIN_A3, BAUD=40000, MODE=2, BITS=16, ENABLE_ACTIVE=1, STREAM=ADC)
    #use spi (MASTER, MODE=0, BITS=8, STREAM=SPI_4)
    */
#endif

#ifdef PIC18
    //#include <18F67J94.h>
    //-------------UART--------------------
    #pin_select TX3=PIN_E3                                                           //TX PIN EXT +/- 6v
    #pin_select RX3=PIN_F2                                                           //RX PIN EXT +/- 6V
    #use rs232(baud=9600, parity=N, UART3, bits=8, stream=BOSS, ERRORS)                      
    
    //#use rs232(baud=9600,parity=N, xmit=PIN_E4,bits=8,stream=PC)
    #use rs232(baud=9600,parity=N, xmit=PIN_B6, bits=8, stream=PC)

    // -------------SPI--------------------
    #define SMF_CS PIN_A2
    #define MIS_FM_CS  PIN_A5

    #pin_select SCK1=PIN_A3
    #pin_select SDI1=PIN_A0
    #pin_select SDO1=PIN_A1
    #define SMF_STREAM MIS_FM_STREAM
    // #define MIS_FM_STREAM FLASH_STREAM
    #use spi (MASTER, CLK=PIN_A3, DI=PIN_A1, DO=PIN_A0, BAUD=200000, MODE=0, BITS=8, STREAM=MIS_FM_STREAM)
    // #use spi (MASTER, SPI1, BAUD=200000, MODE=0, BITS=8, STREAM=MIS_FM_STREAM)
    //#use spi (MASTER, SPI1, BAUD=200000, MODE=0, BITS=8, STREAM=SMF_STREAM)
    //#use spi (MASTER, CLK=PIN_A3, DI=PIN_A0, DO=PIN_A1, BAUD=200000, MODE=0, BITS=8, STREAM=SMF_STREAM)



    

    #use spi (MASTER, CLK=PIN_B2, DI=PIN_B4, DO=PIN_B5,  BAUD=1000000, MODE=0, BITS=16, STREAM=ADCDAC_STREAM)
    #define ADC_CS PIN_B1
    #define ADC_STREAM ADCDAC_STREAM
    #define DAC1_CS PIN_B3
    #define DAC1_STREAM ADCDAC_STREAM
    #define DAC2_CS PIN_B0
    #define DAC2_STREAM ADCDAC_STREAM

    // DI/O___
    #define CONNECT_CIGS1 PIN_C0
    #define CONNECT_CIGS2 PIN_C5
    #define EN_NPWR PIN_C7

#endif
//-------------I/O port--------------------







// ADC Settings
#define ADC_CIGS1_VOLT      AD7490_VIN0
#define ADC_CIGS1_CURR      AD7490_VIN1
#define ADC_CIGS1_AMP       AD7490_VIN2
#define ADC_CIGS1_VREF      AD7490_VIN3
#define ADC_CIGS2_VOLT      AD7490_VIN4
#define ADC_CIGS2_CURR      AD7490_VIN5
#define ADC_CIGS2_AMP       AD7490_VIN6
#define ADC_CIGS2_VREF      AD7490_VIN7
#define ADC_CIGS1_DAC       AD7490_VIN8
#define ADC_CIGS2_DAC       AD7490_VIN9
#define ADC_TEMP_MIS7       AD7490_VIN12
#define ADC_TEMP_PY_TOP     AD7490_VIN13
#define ADC_TEMP_PY_BOT     AD7490_VIN14
#define ADC_PD              AD7490_VIN15


#endif // MMJ_CIGS_CONFIG_H

//------------------End of File------------------
