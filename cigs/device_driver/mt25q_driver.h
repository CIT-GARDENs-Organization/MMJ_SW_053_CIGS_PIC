#ifndef MT25Q_DRIVER_H
#define MT25Q_DRIVER_H

#include "../../lib/device/mt25q copy.h"


typedef enum FLASH_STREAM_ID_t{
    FLASH_STREAM0,
    FLASH_STREAM1,
    FLASH_STREAM2,
    FLASH_STREAM3
} FLASH_STREAM_ID_t;

typedef struct select_stream_to_flash{
    FLASH_STREAM_ID_t flash_stream_id;
    MT25Q_Model_t flash_model;
    int16 cs_pin;
} Flash;

int1 mt25q_is_connect(Flash flash_stream);
void mt25q_read(Flash flash_stream, unsigned int32 read_address, unsigned int8 *read_data, unsigned int32 read_amount);
void mt25q_write(Flash flash_stream, unsigned int32 write_address, unsigned int8 *write_data, unsigned int32 write_amount);
void mt25q_sector_erase(Flash flash_stream, unsigned int32 sector_address); 
mt25q_status_reg_t mt25q_read_status(Flash flash_stream);

#endif // MT25Q_DRIVER_H