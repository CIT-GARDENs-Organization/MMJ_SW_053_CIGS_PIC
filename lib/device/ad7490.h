#ifndef  _AD7490_H_
#define  _AD7490_H_

// === AD7490 Control Word ===
// Bit[15]   : WRITE (常に0)
// Bit[14:13]: SEQ mode (シーケンス設定)
// Bit[12]   : SHADOW (0=disabled, 1=enabled)
// Bit[11]   : Don't Care
// Bit[10]   : PM1 (Power Management)
// Bit[9]    : PM0
// Bit[8]    : REFSEL (0=internal, 1=external)
// Bit[7]    : Coding (0=straight binary, 1=twos complement)
// Bit[6]    : Don't Care
// Bit[5]    : ADD2
// Bit[4]    : ADD1
// Bit[3]    : ADD0
// Bit[2]    : DIFF (0=single-ended, 1=differential)
// Bit[1:0]  : Don't Care



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
#define AD7490_PM_NORMAL 0b00000011
#define AD7490_PM_SHUTDOWN 0b00000001
#define AD7490_PM_AUTO_SHUTDOWN 0b00000010
#define AD7490_PM_AUTO_STANDBY 0b00000000


// ----------------------------
// WRITE bit
// ----------------------------
typedef enum {
    AD7490_WRITE_DISABLE = 0,  // 書き込み無効
    AD7490_WRITE_ENABLE  = 1   // 書き込み有効
} ad7490_write_t;

// ----------------------------
// SEQ bit
// ----------------------------
typedef enum {
    AD7490_SEQ_DISABLE = 0,
    AD7490_SEQ_ENABLE  = 1
} ad7490_seq_t;

// ----------------------------
// CODING bit
// ----------------------------
typedef enum {
    AD7490_CODING_BINARY  = 0, // ストレートバイナリ
    AD7490_CODING_TWOS    = 1  // Two’s complement
} ad7490_coding_t;

// ----------------------------
// RANGE bit
// ----------------------------
typedef enum {
    AD7490_RANGE_2REF  = 0, // 0〜2*REFIN
    AD7490_RANGE_REF   = 1  // 0〜REFIN
} ad7490_range_t;

// ----------------------------
// WEAK/TRI bit
// ----------------------------
typedef enum {
    AD7490_DOUT_TRI   = 0, // Three-state
    AD7490_DOUT_WEAK  = 1  // Weakly driven
} ad7490_dout_t;

// ----------------------------
// SHADOW bit
// ----------------------------
typedef enum {
    AD7490_SHADOW_DISABLE = 0,
    AD7490_SHADOW_ENABLE  = 1
} ad7490_shadow_t;

// ----------------------------
// Power Mode (PM1, PM0)
// ----------------------------
typedef enum {
    AD7490_PWR_NORMAL    = 0, // 00
    AD7490_PWR_FULLDOWN  = 1, // 01
    AD7490_PWR_AUTO      = 2, // 10
    AD7490_PWR_SHUTDOWN  = 3  // 11
} ad7490_pwr_mode_t;

// ----------------------------
// Channel Address (ADD3–ADD0)
// ----------------------------
typedef enum {
    AD7490_CH0  = 0,
    AD7490_CH1  = 1,
    AD7490_CH2  = 2,
    AD7490_CH3  = 3,
    AD7490_CH4  = 4,
    AD7490_CH5  = 5,
    AD7490_CH6  = 6,
    AD7490_CH7  = 7,
    AD7490_CH8  = 8,
    AD7490_CH9  = 9,
    AD7490_CH10 = 10,
    AD7490_CH11 = 11,
    AD7490_CH12 = 12,
    AD7490_CH13 = 13,
    AD7490_CH14 = 14,
    AD7490_CH15 = 15
} ad7490_channel_t;

typedef struct {
    ad7490_write_t     write;
    ad7490_seq_t       seq;
    ad7490_coding_t    coding;
    ad7490_range_t     range;
    ad7490_dout_t      dout;
    ad7490_shadow_t    shadow;
    ad7490_pwr_mode_t  power_mode;
    ad7490_channel_t   channel;
} ad7490_config_t;


unsigned int16 ad7490_make_cmd(ad7490_config_t config);


#endif // _AD7490_H_
//------------------End of File------------------
