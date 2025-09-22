#ifndef MT25Q_COPY_H
#define MT25Q_COPY_H

// =========
//#define MT25Q_DEBUG
// =========

#define MT25QL128ABA 0x00     //Mission Flash
#define MT25QL01GBBB 0x01     //SMF,CF
typedef struct select_stream_to_flash{
    SpiStreamId spi_stream_id;
    unsigned int8 flash_model;
    int16 cs_pin;
}Flash;

#define MT25Q_PAGE_SIZE 256


// ==================== cmd =========================
#define MT25Q_CMD_SIZE 5

enum cmd_type_type{
   MT25Q_READ,
   MT25Q_WRITE,
   MT25Q_ERASE,
};

typedef struct {
    union {
        unsigned int8 all[MT25Q_CMD_SIZE]; // コマンドバイト列
        struct {
            unsigned int8 cmd;              // コマンド
            unsigned int8 addr[4];          // アドレス（3バイトまたは4バイト）
        } parts;
    }data;
    unsigned int8 length;               // コマンド長
} MT25Q_Cmd_t;

// MT25Q SPI Commands
typedef enum {
    // Status Registers
    MT25Q_READ_STATUS1       = 0x05,
    MT25Q_READ_STATUS2       = 0x35,
    MT25Q_WRITE_ENABLE       = 0x06,
    MT25Q_WRITE_DISABLE      = 0x04,

    // ID
    MT25Q_READ_ID            = 0x9F,
    MT25Q_READ_MFG_DEVICE_ID = 0x90,

    // Read
    MT25Q_READ               = 0x03, // 3-byte address
    MT25Q_FAST_READ          = 0x0B,
    MT25Q_4BYTE_READ         = 0x13, // 4-byte address
    MT25Q_4BYTE_FAST_READ    = 0x0C,

    // Program
    MT25Q_PAGE_PROGRAM       = 0x02, // 3-byte address
    MT25Q_4BYTE_PAGE_PROGRAM = 0x12, // 4-byte address

    // Erase
    MT25Q_SUBSECTOR_4KB_ERASE     = 0x20, // 3-byte address
    MT25Q_SUBSECTOR_32KB_ERASE    = 0x52, // 3-byte address
    MT25Q_SECTOR_ERASE             = 0xD8, // 64KB 3-byte address
    MT25Q_4BYTE_SUBSECTOR_4KB_ERASE  = 0x21, // 4-byte address
    MT25Q_4BYTE_SUBSECTOR_32KB_ERASE = 0x5C, // 4-byte address
    MT25Q_4BYTE_SECTOR_ERASE          = 0x21, // 4-byte address
    MT25Q_CHIP_ERASE                 = 0xC7, // 全チップ消去

    // 4-byte address mode
    MT25Q_ENTER_4BYTE_ADDR_MODE = 0xB7,
    MT25Q_EXIT_4BYTE_ADDR_MODE  = 0xE9
} MT25Q_CmdCode_t;


typedef enum {
    MT25QL128ABA, // 3バイトアドレス
    MT25QL01GBBB  // 4バイトアドレス
} MT25Q_Model_t;

MT25Q_Cmd_t mt25q_make_cmd(enum MT25Q_CmdCode_t cmd_type, unsigned long address);


typedef union {
    unsigned int8 byte;   // ステータスレジスタ全体
    struct {
        unsigned int1 WIP  : 1; // Bit 0: Write In Progress
        unsigned int1 WEL  : 1; // Bit 1: Write Enable Latch
        unsigned int1 BP0  : 1; // Bit 2: Block Protect 0
        unsigned int1 BP1  : 1; // Bit 3: Block Protect 1
        unsigned int1 TB   : 1; // Bit 4: Top/Bottom Protect
        unsigned int1 SEC  : 1; // Bit 5: Sector Protect
        unsigned int1 SRP0 : 1; // Bit 6: Status Register Protect 0
        unsigned int1 SRP1 : 1; // Bit 7: Status Register Protect 1
    } bits;
} mt25q_status_reg_t;


typedef union {
    unsigned int8 bytes[3];  // 生のIDバイト列
    struct {
        unsigned int8 manufacturer_id; // メーカーID
        unsigned int8 memory_type;     // メモリタイプ
        unsigned int8 capacity;        // 容量
    } fields;
} MT25Q_ChipID_t;

#define MT25Q_MANUFACTURER_ID_MICRON 0x20 // MicronのメーカーID
#define MT25Q_READ_ID_DATASIZE 3 // Read IDのデータサイズ（バイト）

#endif // MT25Q_COPY_H