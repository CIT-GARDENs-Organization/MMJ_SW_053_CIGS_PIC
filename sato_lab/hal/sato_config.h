#ifndef MMJ_CIGS_CONFIG_H
#define MMJ_CIGS_CONFIG_H




#include <18F67J94.h>
//#include <16F1789.h>
#opt 0 // 0 = no optimization, 1 = optimize for speed, 2 = optimize for size

#device ADC=12
#use delay(crystal=16MHz)
#fuses HS, NOWDT, NOBROWNOUT, NOPROTECT
#device HIGH_INTS=TRUE
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
    // BOSS command port
    #pin_select TX3=PIN_E3                                                          
    #pin_select RX3=PIN_F2                                                        
    #use rs232(baud=9600, parity=N, UART3, bits=8, stream=BOSS, ERRORS)                      
    
    // PC debug port
    #pin_select TX4=PIN_E5
    #pin_select RX4=PIN_D4  
    #use rs232(baud=9600,parity=N, UART4, bits=8,stream=PC, ERRORS)

    // -------------SPI--------------------
    // Shared Mission Flash
    #define SMF_CS PIN_A2
    #pin_select SCK1OUT=PIN_A3
    #pin_select SDI1=PIN_A0
    #pin_select SDO1=PIN_A1
    // #use spi (MASTER, SPI1, BAUD=200000, MODE=0, BITS=8, STREAM=SMF_STREAM)
    #use spi (MASTER, CLK=PIN_A3, DI=PIN_A1, DO=PIN_A0, BAUD=20000000, MODE=0, BITS=8, STREAM=SMF_STREAM)


    // Mission Flash
    #define MIS_FM_CS  PIN_G0
    #use spi (MASTER, CLK=PIN_E1, DI=PIN_E0, DO=PIN_E6, BAUD=20000000, MODE=0, BITS=8, STREAM=MIS_FM_STREAM)


    // ADC  
    #define ADC_CS PIN_B1
    // #pin_select SCK2OUT=PIN_B2
    // #pin_select SDI2=PIN_B4
    // #pin_select SDO2=PIN_B5
    // #use spi (MASTER, SPI2, BAUD=200000, MODE=2, BITS=16, STREAM=ADC_STREAM)
    #use spi (MASTER, CLK=PIN_B2, DI=PIN_B4, DO=PIN_B5, BAUD=20000000, MODE=2, BITS=16, STREAM=ADC_STREAM)

    // DAC
    #define DAC1_CS PIN_D7
    #define DAC2_CS PIN_D6
    #use spi (MASTER, CLK=PIN_D5, DO=PIN_D4,  BAUD=20000000, MODE=2, BITS=16, STREAM=DAC_STREAM)


    // DI/O___
    #define CONNECT_CIGS1 PIN_C3
    #define CONNECT_CIGS2 PIN_C5
    #define EN_NPWR1 PIN_C7
    #define EN_NPWR2 PIN_C6

#endif

//-------------I/O port--------------------
#use FIXED_IO( A_outputs=SMF_CS )
#use FIXED_IO( B_outputs=ADC_CS )
#use FIXED_IO( C_outputs=CONNECT_CIGS1, CONNECT_CIGS2, EN_NPWR1, EN_NPWR2 )
#use FIXED_IO( D_outputs=DAC1_CS, DAC2_CS )
#use FIXED_IO( G_outputs=MIS_FM_CS )

#define ADC_CIGS1_VOLT      AD7490_CH0
#define ADC_CIGS1_CURR      AD7490_CH1
#define ADC_CIGS1_AMP       AD7490_CH2
#define ADC_CIGS1_VREF      AD7490_CH3
#define ADC_CIGS2_VOLT      AD7490_CH4
#define ADC_CIGS2_CURR      AD7490_CH5
#define ADC_CIGS2_AMP       AD7490_CH6
#define ADC_CIGS2_VREF      AD7490_CH7
#define ADC_CIGS1_DAC       AD7490_CH8
#define ADC_CIGS2_DAC       AD7490_CH9
#define EMPTY_VIN0          AD7490_CH10
#define EMPTY_VIN1          AD7490_CH11
#define ADC_TEMP_MIS7       AD7490_CH12
#define ADC_TEMP_PY_TOP     AD7490_CH13
#define ADC_TEMP_PY_BOT     AD7490_CH14
#define ADC_PD              AD7490_CH15

// ADC Settings
#define ADC_DIODE_INNER_VOLT   ADC_CIGS1_VOLT  
#define ADC_DIODE_INNER_CURR    ADC_CIGS1_CURR 
#define ADC_DIODE_INNER_AMP    ADC_CIGS1_AMP   
#define ADC_DIODE_INNER_VREF   ADC_CIGS1_VREF  
#define ADC_DIODE_INNER_VOLT   ADC_CIGS2_VOLT  
#define ADC_DIODE_OUTER_CURR   ADC_CIGS2_CURR  
#define ADC_DIODE_OUTER_AMP    ADC_CIGS2_AMP   
#define ADC_DIODE_OUTER_VREF   ADC_CIGS2_VREF  
#define ADC_DIODE_INNER_DAC    ADC_CIGS1_DAC   
#define ADC_DIODE_OUTER_DAC    ADC_CIGS2_DAC   


#endif // MMJ_CIGS_CONFIG_H

//------------------End of File------------------
