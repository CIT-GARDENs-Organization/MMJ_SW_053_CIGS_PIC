#include "mt25q_driver.h"
#include "../../lib/device/mt25q copy.h"

void mt25q_read(Flash flash_stream, unsigned int32 read_address, unsigned int8 *read_data, unsigned int32 read_amount)
{
    for (unsigned int32 count = 0; count < read_amount; count++) {
        MT25Q_Cmd_t flash_cmd = mt25q_make_cmd(flash_stream.flash_model, MT25Q_READ, read_address);
        output_low(flash_stream.cs_pin);
        spi_xfer(MISF_STREAM, flash_cmd.data.all, flash_cmd.length);
        read_data[count] = spi_xfer(MISF_STREAM, 0x00); // ダミーバイトを送信してデータを受信
        output_high(flash_stream.cs_pin);
    }
    return;
}

void mt25q_write(Flash flash_stream, unsigned int32 write_start_address, int8 *write_data_ptr, unsigned int16 write_amount) 
{
    unsigned int16 remaining = write_amount;
    unsigned int32 addr = write_start_address;

    while (remaining > 0) {
        // 現在ページ内で書き込めるサイズ
        unsigned int16 page_offset = addr % MT25Q_PAGE_SIZE;
        unsigned int16 bytes_in_page = MT25Q_PAGE_SIZE - page_offset;
        unsigned int16 write_size = (remaining < bytes_in_page) ? remaining : bytes_in_page;

        // Write Enable
        MT25Q_Cmd_t flash_cmd = mt25q_make_cmd(flash_stream.flash_model, MT25Q_WRITE_ENABLE, 0);
        output_low(flash_stream.cs_pin);
        mt25q_select_stream(flash_stream, flash_cmd.data.all, flash_cmd.length);
        output_high(flash_stream.cs_pin);

        // Write Data
        flash_cmd = mt25q_make_cmd(flash_stream.flash_model, MT25Q_WRITE, addr);
        output_low(flash_stream.cs_pin);
        mt25q_select_stream(flash_stream, write_data_ptr, write_size);
        output_high(flash_stream.cs_pin);

        // 書き込み完了待ち
        mt25q_status_reg_t mt25q_status;
        do {
            mt25q_status = mt25q_read_status(flash_stream.flash_model); 
        } while (mt25q_status.bits.WIP);

        // 次ページへ
        remaining -= write_size;
        data_ptr += write_size;
        addr += write_size;
    }
}

void mt25q_select_stream(Flash flash_stream, unsigned int8 *write_data_ptr, unsigned int16 write_amount) {
    switch (flash_stream.flash_stream_id) {
        case FLASH_STREAM0:
            for (unsigned int16 spi_xfer_num = 0; spi_xfer_num < write_amount; spi_xfer_num++)
                spi_xfer(FLASH_STREAM0, write_data_ptr[spi_xfer_num]);
            break;
        case FLASH_STREAM1:
            for (unsigned int16 spi_xfer_num = 0; spi_xfer_num < write_amount; spi_xfer_num++)
                spi_xfer(FLASH_STREAM1, write_data_ptr[spi_xfer_num]);
            break;
        case FLASH_STREAM2:
            for (unsigned int16 spi_xfer_num = 0; spi_xfer_num < write_amount; spi_xfer_num++)
                spi_xfer(FLASH_STREAM2, write_data_ptr[spi_xfer_num]);
            break;
        case FLASH_STREAM3:
            for (unsigned int16 spi_xfer_num = 0; spi_xfer_num < write_amount; spi_xfer_num++)
                spi_xfer(FLASH_STREAM3, write_data_ptr[spi_xfer_num]);
            break;
        default:
            break;
    }
    return;
}

void mt25q_select_stream(Flash flash_stream, unsigned int8 *write_data_ptr, unsigned int16 write_amount, unsigned int8 *read_data_ptr, unsigned int32 read_amount) {
    switch (flash_stream.flash_stream_id) {
        case FLASH_STREAM0:
            for (unsigned int16 spi_xfer_num = 0; spi_xfer_num < write_amount; spi_xfer_num++)
                read_data_ptr[spi_xfer_num] = spi_xfer(FLASH_STREAM0, write_data_ptr[spi_xfer_num]);
            break;
        case FLASH_STREAM1:
            for (unsigned int16 spi_xfer_num = 0; spi_xfer_num < write_amount; spi_xfer_num++)
                read_data_ptr[spi_xfer_num] = spi_xfer(FLASH_STREAM1, write_data_ptr[spi_xfer_num]);
            break;
        case FLASH_STREAM2:
            for (unsigned int16 spi_xfer_num = 0; spi_xfer_num < write_amount; spi_xfer_num++)
                read_data_ptr[spi_xfer_num] = spi_xfer(FLASH_STREAM2, write_data_ptr[spi_xfer_num]);
            break;
        case FLASH_STREAM3:
            for (unsigned int16 spi_xfer_num = 0; spi_xfer_num < write_amount; spi_xfer_num++)
                read_data_ptr[spi_xfer_num] = spi_xfer(FLASH_STREAM3, write_data_ptr[spi_xfer_num]);
            break;
        default:
            break;
    }
    return;
}

int1 mt25q_is_connect(Flash flash_stream) {
    MT25Q_ChipID_t chip_id;
    unsigned int8 flash_cmd = MT25Q_READ_ID;
    output_low(flash_stream.cs_pin);
    mt25q_select_stream(flash_stream, &flash_cmd, 1, chip_id.bytes, MT25Q_READ_ID_DATASIZE);
    output_high(flash_stream.cs_pin);
    // chip id check
    if (chip_id.fields.manufacturer_id == MT25Q_MANUFACTURER_ID_MICRON) {
        return true;
    } else {
        return false;
    }
}

mt25q_status_reg_t mt25q_read_status(Flash flash_stream)
{
    mt25q_status_reg_t status_reg;
    unsigned int8 flash_cmd = MT25Q_READ_STATUS1;
    output_low(flash_stream.cs_pin); // lower the CS PIN
    mt25q_select_stream(flash_stream, &flash_cmd, 1, &status_reg.byte, 1);
    output_high(flash_stream.cs_pin); // take CS PIN higher back
    return status_reg;
}