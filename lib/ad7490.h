#ifndef     AD7490_H
#define     AD7490_H

#ifdef AD7490_DEBUG
   #define DEBUG_PORT PC
#endif

typedef struct AD7490_STREAM
{
    int8 spi_stream; // SPI stream number
    int16 cs_pin;    // Chip Select pin number
};
//0x8B30



typedef union AD7490_RESISTOR
{
    unsigned int16 value; // 16-bit value for direct access
    struct fields
    {
        unsigned int blank3 :1 ; // bit 0 (LSB)
        unsigned int blank2 :1   ; // bit 1
        unsigned int blank1 :1   ; // bit 2
        unsigned int blank0 :1   ; // bit 3
        unsigned int coding:1       ; // bit 12
        unsigned int range   :1    ; // bit 5
        unsigned int weak_tri:1     ; // bit 4
        unsigned int shadow:1   ; // bit 13
        unsigned int write:1    ; // bit 15 (MSB)
        unsigned int seq :1     ; // bit 14
        unsigned int add3 :1    ; // bit 11
        unsigned int add2  :1   ; // bit 10
        unsigned int add1   :1  ; // bit 9
        unsigned int add0    :1 ; // bit 8
        unsigned int pm1    :1  ; // bit 7
        unsigned int pm0     :1 ; // bit 6
    } fields;
} AD7490_RESISTOR;

unsigned int8 coding; // 1: Two's complement, 0: Binary
unsigned int8 write;  // 1: Write operation, 0: Read operation
unsigned int8 seq;    // 1: Sequential mode, 0: Random mode
unsigned int8 range; // 1: Range mode, 0: Normal mode
unsigned int8 weak_tri; // 1: Weak trigger, 0: Strong trigger
unsigned int8 shadow; // 1: Shadow register, 0: Direct access
unsigned int16 ad7490_readdata(unsigned int16 channel);
//AD7490_RESISTOR reg;

AD7490_RESISTOR reg = {0}; // Initialize the AD7490_RESISTOR union

// Public Function 
void ad7490_init(void);
unsigned int16 ad7490_read(int8 channel);

unsigned int16 ad7490_make_cmd(int8 channel);

// Private Functions
void set_add(int8 channel);
void set_pm(int8 powermode);
void print_bits(unsigned int16 data);
// Resistor Settings
#define AD7490_EN_WRITE 0b1

// Power Mode Register
#define AD7490_PM_NORMAL 0b00000000
#define AD7490_PM_SHUTDOWN 0b00000001
#define AD7490_PM_AUTO_SHUTDOWN 0b00000010
#define AD7490_PM_AUTO_STANDBY 0b00000011


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
