#include "../mmj_cigs_piclog.h"
#include "../../lib/timer.h"




void piclog_make(unsigned int8 function, unsigned int8 parameter)
{
    PICLOG piclog_data;
    piclog_data.time = get_current_sec();
    piclog_data.function = function;
    piclog_data.parameter = parameter;
    /*
    // フラッシュ書き込み
    piclog_write(piclog_data.bytes);
    fprintf(PC, "PICLOG data written: Function %d, Parameter %d\r\n", function, parameter);
    piclog_data_counter += sizeof(PICLOG);
    if (piclog_data_counter >= PICLOG_PACKET_SIZE)
    {
        spi_xfer_write(SMF_CS, piclog_data.bytes, sizeof(PICLOG)); // SPIでフラッシュに書き込み
        //unsigned int8 crc = calc_crc8(piclog_data.bytes, sizeof(PICLOG) - 1); // CRC計算
        piclog_data_counter = 0; // リセット
    }
    */
}

void piclog_write(unsigned int8 *data)
{
    /*
    if (flash_isconnect(SMF_STREM))
    {
        fprintf(PC, "PICLOG data write\r\n");
        unsigned int8 data[sizeof(PICLOG)];
        //flash_write(ADDRESS_PICLOG_START, *piclog_data[bytes],8); // フラッシュに書き込み
    }
    else
    {
        fprintf(PC, "Flash is not connected\r\n");
        return;
    }
    */
}
    
