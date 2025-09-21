#ifndef MT25Q_H
#define MT25Q_H

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


// ==================== cmd =========================
#define MT25Q_CMD_SIZE 5
unsigned int8 make_mt25q_cmd[MT25Q_CMD_SIZE];

enum cmd_type_type{
   MT25Q_READ,
   MT25Q_WRITE,
   MT25Q_ERASE,
};
