#ifndef MT25Q_H
#define MT25Q_H

// =========
//#define MT25Q_DEBUG
// =========


#define FLASH_STREAM0 MIS_FM_STREAM     // <- Align the names to `config.h` Stream name
#define FLASH_STREAM1 SMF_STREAM
#define FLASH_STREAM2 SMF_STREAM // dont use
#define FLASH_STREAM3 SMF_STREAM // dont use

typedef enum spi_stream{
    SPI_0 = 0,
    SPI_1,
    SPI_2,
    SPI_3
}SpiStreamId;

#define MT25QL128ABA 0x00     //Mission Flash
#define MT25QL01GBBB 0x01     //SMF,CF
typedef struct select_stream_to_flash{
    SpiStreamId spi_stream_id;
    unsigned int8 flash_model;
    int16 cs_pin;
}Flash;


Flash mis_fm = {SPI_0, MT25QL128ABA, MIS_FM_CS};
Flash smf = {SPI_1, MT25QL01GBBB, SMF_CS};



// ===================== Function List =====================
// Public Functions

int1 is_connect(Flash flash_stream);

void write_data_bytes(Flash flash_stream, unsigned int32 write_start_address, unsigned int8 *write_data, unsigned int16 write_amount);      

void read_data_bytes(Flash flash_stream, unsigned int32 read_start_address, unsigned int8 *read_data, unsigned int32 read_amount);  

void sector_erase(Flash flash_stream, unsigned int32 sector_address);

void subsector_32kByte_erase(Flash flash_stream, unsigned int32 subsector_address);  
                               
void subsector_4kByte_erase(Flash flash_stream, unsigned int32 subsector_address);      

//Private Functions
 
void flash_setting(Flash flash_stream);
 
int8 status_register(Flash flash_stream);
 
int8 read_id(Flash flash_stream);

unsigned int8 read_data_byte(Flash flash_stream, unsigned int32 read_address);

void write_data_byte(Flash flash_stream, unsigned int32 write_address, unsigned int8 write_data);



// ===================== Flash Commands =====================
#define CMD_READ_ID                     0x9F
#define CMD_READ_STATUS_REGISTER        0x05
#define CMD_READ                        0x03//for MT25QL128ABA
#define CMD_4BYTE_READ                  0x13//for MT25QL01GBBB
#define CMD_WRITE_ENABLE                0x06
#define CMD_PAGE_PROGRAM                0x02//for MT25QL128ABA
#define CMD_4BYTE_PAGE_PROGRAM          0x12//for MT25QL01GBBB
#define CMD_SUBSECTOR_4KB_ERASE         0x20//for MT25QL128ABA
#define CMD_4BYTE_SUBSECTOR_4KB_ERASE   0x21//for MT25QL01GBBB
#define CMD_SUBSECTOR_32KB_ERASE        0x52//for MT25QL128ABA
#define CMD_4BYTE_SUBSECTOR_32KB_ERASE  0x5C//for MT25QL01GBBB
#define CMD_SECTOR_ERASE                0xD8//for MT25QL128ABA
#define CMD_4BYTE_SECTOR_ERASE          0xDC//for MT25QL01GBBB

// ====================== Value List =====================
#define MANUFACTURER_ID_MICRON 0x20 // Manufacturer ID for Micron flash
#define READ_ID_DATASIZE 20 // 20 bytes for read ID data


// ====================== Data Structures =====================
typedef union
{
    unsigned int8 bytes[READ_ID_DATASIZE]; // 20 bytes for read ID
    struct {
        unsigned int8 manufacturer_id; // 1 byte
        unsigned int8 memory_type;     // 1 byte
        unsigned int8 capacity;        // 1 byte
        unsigned int8 reserved;       // 1 byte
    } fields;
}READ_ID_DATA;

// ================================== ADD NEW DEFINES HERE ==================================
// =============================================================================================
// MT25Q Flash Memory Specifications
#define MT25Q_BLOCK_SIZE   4096
#define MT25Q_BLOCK_NUM    8192
#define DEV
#ifdef DEV
typedef enum mt25q_cmd{
    //Software RESET Operations
    MT25Q_CMD_RESET_ENABLE = 0x66,
    MT25Q_CMD_RESET_MEMORY = 0x99,

    //READ ID Operations
    MT25Q_CMD_READ_ID = 0x9F,
    MT25Q_CMD_READ_ID_MULTI_IO = 0xAF,
    MT25Q_CMD_READ_PARAMETER = 0x5A,

    //READ MEMORY Operations
    MT25Q_CMD_READ = 0x03,
    MT25Q_CMD_READ_FAST = 0x0B,
    MT25Q_CMD_READ_DUAL_OUTPUT_FAST = 0x3B,
    MT25Q_CMD_READ_DUAL_IO_FAST = 0xBB,
    MT25Q_CMD_READ_QUAD_OUTPUT_FAST = 0x6B,
    MT25Q_CMD_READ_QUAD_IO_FAST = 0xEB,
    MT25Q_CMD_READ_QUAD_IO = 0x6B,
    MT25Q_CMD_READ_DTR_FAST = 0x0D,
    MT25Q_CMD_READ_DTR_OUTPUT_FAST = 0x3D,
    MT25Q_CMD_READ_DTR_IO_FAST = 0xBD,
    MT25Q_CMD_READ_DTR_QUAD_OUTPUT_FAST = 0x6D,
    MT25Q_CMD_READ_DTR_QUAD_IO_FAST = 0xED, 
    MT25Q_CMD_READ_QUAD_IO_WORD = 0xE7,

    //READ MEMORY Operations with 4-Byte Address
    MT25Q_CMD_4BYTE_READ = 0x13,
    MT25Q_CMD_4BYTE_READ_FAST = 0x0C,
    MT25Q_CMD_4BYTE_READ_DUAL_OUTPUT_FAST = 0x3C,
    MT25Q_CMD_4BYTE_DUAL_IO_FAST_READ = 0xBC,  
    MT25Q_CMD_4BYTE_QUAD_OUTPUT_FAST_READ = 0x6C,
    MT25Q_CMD_4BYTE_QUAD_IO_FAST_READ = 0xEC,   
    MT25Q_CMD_4BYTE_DTR_FAST_READ = 0x0E,
    MT25Q_CMD_4BYTE_DTR_DUAL_IO_FAST_READ = 0xBE,
    MT25Q_CMD_4BYTE_DTR_QUAD_IO_FAST_READ = 0xEE,

    //WRITE Operations
    MT25Q_CMD_WRITE_ENABLE = 0x06,
    MT25Q_CMD_WRITE_DISABLE = 0x04,

    //READ REGISTER Operations
    MT25Q_CMD_READ_STATUS_REG = 0x05,
    MT25Q_CMD_READ_FLAG_STATUS_REG = 0x70,
    MT25Q_CMD_READ_NONVOL_CFG_REG = 0xB5,
    MT25Q_CMD_READ_VOL_CFG_REG = 0x85,
    MT25Q_CMD_READ_ENHANCED_VOL_CFG_REG = 0x65,
    MT25Q_CMD_READ_EXT_ADDR_REG = 0xC8,
    MT25Q_CMD_READ_GENERAL_PURPOSE_REG = 0x96,

    //WRITE REGISTER Operations
    MT25Q_CMD_WRITE_STATUS_REG = 0x01,
    MT25Q_CMD_WRITE_NONVOL_CFG_REG = 0xB1,
    MT25Q_CMD_WRITE_VOL_CFG_REG = 0x81,
    MT25Q_CMD_WRITE_ENHANCED_VOL_CFG_REG = 0x61,
    MT25Q_CMD_WRITE_EXT_ADDR_REG = 0xC5,

    //CLEAR FLAG STATUS REGISTER Operation
    MT25Q_CMD_CLEAR_FLAG_STATUS_REG = 0x50,

    //PROGRAM Operations
    MT25Q_CMD_PAGE_PROGRAM = 0x02,
    MT25Q_CMD_DUAL_INPUT_FAST_PROGRAM = 0xA2,
    MT25Q_CMD_EXT_DUAL_INPUT_FAST_PROGRAM = 0xD2,
    MT25Q_CMD_QUAD_INPUT_FAST_PROGRAM = 0x32,
    MT25Q_CMD_EXT_QUAD_INPUT_FAST_PROGRAM = 0x38,

    //PROGRAM Operations with 4-Byte Address
    MT25Q_CMD_4BYTE_PAGE_PROGRAM = 0x12,
    MT25Q_CMD_4BYTE_QUAD_INPUT_FAST_PROGRAM = 0x34,
    MT25Q_CMD_4BYTE_QUAD_INPUT_EXT_FAST_PROGRAM = 0x3E,

    //ERASE Operations
    MT25Q_CMD_ERASE_32KB_SUBSECTOR = 0x52,
    MT25Q_CMD_ERASE_4KB_SUBSECTOR = 0x20,
    MT25Q_CMD_ERASE_SECTOR = 0xD8,
    MT25Q_CMD_ERASE_BULK = 0xC7,

    //ERASE Operations with 4-Byte Address
    MT25Q_CMD_4BYTE_ERASE_SECTOR = 0xDC,
    MT25Q_CMD_4BYTE_ERASE_4KB_SUBSECTOR = 0x21,
    
    //SUSPEND/RESUME Operations
    MT25Q_CMD_PROGRAM_ERASE_SUSPEND = 0x75,
    MT25Q_CMD_PROGRAM_ERASE_RESUME = 0x7A,

    //ONE-WIRE PROGRAMMING Operations
    MT25Q_CMD_READ_OPT_ARRAY = 0x4B,
    MT25Q_CMD_PROGRAM_OPT_ARRYA = 0x42,

    //4-BYTE ADDRESSING Operations
    MT25Q_CMD_ENTER_4BYTE_ADDR_MODE = 0xB7,
    MT25Q_CMD_EXIT_4BYTE_ADDR_MODE = 0xE9,

    //QUAD PROTOCOL Operations
    MT25Q_CMD_ENTER_QUAD_IO_MODE = 0x35,
    MT25Q_CMD_RESET_QUAD_IO_MODE = 0xF5,

    //DEEP POWER-DOWN Operations
    MT25Q_CMD_DEEP_POWER_DOWN = 0xB9,
    MT25Q_CMD_RELEASE_FROM_DEEP_POWER_DOWN = 0xAB,

    //ADVANCED PROGRAM/ERASE CONTROLS
    MT25Q_CMD_READ_SECTOR_PROTECTION = 0x2D,
    MT25Q_CMD_PROGRAM_SECTOR_PROTECTION = 0x2C,
    MT25Q_CMD_READ_VOLATILE_LOCK_BITS = 0xE8,
    MT25Q_CMD_WRITE_VOLATILE_LOCK_BITS = 0xE5,
    MT25Q_CMD_READ_NONVOLATILE_LOCK_BITS = 0xE2,
    MT25Q_CMD_WRITE_NONVOLATILE_LOCK_BITS = 0xE3,
    MT25Q_CMD_READ_GLOBAL_FREEZE_BIT = 0xA7,
    MT25Q_CMD_WRITE_GLOBAL_FREEZE_BIT = 0xA6,
    MT25Q_CMD_READ_PASSWORD = 0x27,
    MT25Q_CMD_WRITE_PASSWORD = 0x28,
    MT25Q_CMD_UNLOCK_PASSWORD = 0x29,

    //ADVANCED SECTOR PROTECTION Operations
    MT25Q_CMD_4BYTE_READ_VOLATILE_LOCK_BITS = 0xE0,
    MT25Q_CMD_4BYTE_WRITE_VOLATILE_LOCK_BITS = 0xE1,

    //ADVANCED FUNCTIONS INTERFACE Operations
    MT25Q_CMD_INTERFACE_ACTIVATION = 0x98,
    MT25Q_CMD_CYCLIC_REDUNDANCY_CHECK = 0x9B
} mt25q_cmd_t;


typedef union {
    unsigned int8 all;  // 全体アクセス
    struct {
        unsigned int8 ADDRESSING_4BYTE     : 1; // Bit0
        unsigned int8 PROTECTION_ERROR     : 1; // Bit1
        unsigned int8 PROGRAM_SUSPEND      : 1; // Bit2
        unsigned int8 RESERVED             : 1; // Bit3
        unsigned int8 PROGRAM_FAIL         : 1; // Bit4
        unsigned int8 ERASE_FAIL           : 1; // Bit5
        unsigned int8 ERASE_SUSPEND        : 1; // Bit6
        unsigned int8 PROGRAM_OR_ERASE_BUSY: 1; // Bit7
    } bit;
} mt25q_sr1_t;

typedef enum {
    MT25QL64,
    MT25QL128,
    MT25QL256,
    MT25QL512,
    MT25QL01G
} mt25q_model_t;


typedef struct {
    mt25q_model_t model;
    unsigned int32 size_bytes;
    unsigned int32 page_size_bytes;
    unsigned int32 sector_size_bytes;
    unsigned int32 subsector_32k_size_bytes;
    unsigned int32 subsector_4k_size_bytes;
} mt25q_info_t;

typedef enum {
    MT25Q_FUNC_READ,
    MT25Q_FUNC_WRITE,
    MT25Q_FUNC_ERASE
} mt25q_func_t;

typedef struct {
    mt25q_func_t function;
    unsigned int32 address;
    unsigned int8 *data;
    unsigned int32 length;
} mt25q_cmd_param_t;

#define MT25Q_CMD_MAX_SIZE 8
typedef struct {
    unsigned int8 cmd[MT25Q_CMD_MAX_SIZE];
    unsigned int8 cmd_length;
} mt25q_send_cmd_t;

// void mt25q_make_cmd(mt25q_info_t *flash_info, mt25q_cmd_param_t *cmd_param, mt25q_send_cmd_t *cmd)


#endif // DEV
#endif