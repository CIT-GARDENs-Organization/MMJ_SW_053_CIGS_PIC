#include <16LF1789.h>
#device ADC=12
#use delay(crystal=16MHz)
#fuses NOMCLR

//

//-------------Communication--------------------
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=BOSS)
#use rs232(baud=9600,parity=N,xmit=PIN_A3,rcv=None,bits=8,stream=PC,FORCE_SW)

#use spi (MASTER, SPI1, ENABLE=PIN_C1, BAUD=400000, MODE=0, BITS=8, STREAM=SMF)
#use spi (MASTER, SPI1, ENABLE=PIN_C2, BAUD=400000, MODE=0, BITS=8, STREAM=PICF)


//-------------I/O port--------------------
// AI/O___ADC
#define TEMP_TOP PIN_A5
#define TEMP_BOT PIN_A3
#define CIGS_VOLT PIN_A0
#define CIGS_CURR PIN_A1
#define PD PIN_E0

// AI/O___DAC
#define Load PIN_A2
#define Load PIN_A2

// DI/O___






