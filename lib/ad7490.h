#ifndef     AD7490_H
#define     AD7490_H

#ifdef AD7490_DEBUG
   #define DEBUG_PORT PC
#endif

struct AD7490_STREAM
{
    int8 spi_stream; // SPI stream number
    int16 cs_pin;    // Chip Select pin number
};


union AD7490_RESISTOR
{
    struct 
    {
        int1 coding; // 1: Two's complement, 0: Binary
        int1 randge;   // 0: 0~VREF, 1: 0~2xVREF
        int1 weak_tri; // 1: Weak trigger, 0: Strong trigger
        int1 shadow;   // 1: Shadow register, 0: Direct access
        int1 pm0;
        int1 pm1; 
        int1 add0;
        int1 add1;    
        int1 add2;    
        int1 add3;
        int1 seq;     // 1: Sequential, 0: Non-sequential
        int1 write;   // 1: Write
        int1 blank0; // 1: Blank, 0: Not blank
        int1 blank1; // 1: Blank, 0: Not blank
        int1 blank2; // 1: Blank, 0: Not blank
        int1 blank3; // 1: Blank, 0: Not blank
    };
    unsigned int16 value; // 16-bit value for direct access
};

// Public Function 
void ad7490_init(void);
unsigned int16 ad7490_read(int16 channel);

// Private Functions
void set_add(int16 channel);
void set_pm(int8 powermode);

// Resistor Settings
#define AD7490_EN_WRITE 0b1

// Power Mode Register
#define AD7490_PM_NORMAL 0b00
#define AD7490_PM_SHUTDOWN 0b01
#define AD7490_PM_AUTO_SHUTDOWN 0b10
#define AD7490_PM_AUTO_STANDBY 0b11


// ADC Channel Register
#define AD7490_VIN0 0b0000
#define AD7490_VIN1 0b0001
#define AD7490_VIN2 0b0010
#define AD7490_VIN3 0b0011
#define AD7490_VIN4 0b0100
#define AD7490_VIN5 0b0101
#define AD7490_VIN6 0b0110
#define AD7490_VIN7 0b0111
#define AD7490_VIN8 0b1000
#define AD7490_VIN9 0b1001
#define AD7490_VIN10 0b1010
#define AD7490_VIN11 0b1011
#define AD7490_VIN12 0b1100
#define AD7490_VIN13 0b1101
#define AD7490_VIN14 0b1110
#define AD7490_VIN15 0b1111


#include "src/ad7490.c"

#endif // AD7490_H
//------------------End of File------------------
