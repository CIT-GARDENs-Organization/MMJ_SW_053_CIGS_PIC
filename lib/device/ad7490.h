#ifndef  AD7490_H
#define  AD7490_H


typedef union {
    unsigned int16 value; // 16-bit value for direct access
    struct {
        int1 reserved0; // Reserved bits
        int1 reserved1; // Reserved bits
        int1 reserved2; // Reserved bits
        int1 reserved3; // Reserved bits   
        int1 coding; // 1: Two's complement, 0: Binary
        int1 range; // 1: Range mode, 0: Normal mode
        int1 weak_tri; // 1: Weak trigger, 0: Strong trigger
        int1 shadow; // 1: Shadow register, 0: Direct access
        int1 pm0;
        int1 pm1; // Power mode bits
        int1 add0;
        int1 add1; // Channel address bits
        int1 add2;
        int1 add3; // Channel address bits
        int1 seq; // 1: Sequential mode, 0: Random mode
        int1 write; // 1: Write operation, 0: Read operation

    } fields;
} ad7490_cmd_t;

// Public Function 
//void ad7490_init(void);
unsigned int16 ad7490_make_cmd(int8 channel);


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




#endif // AD7490_H
//------------------End of File------------------
