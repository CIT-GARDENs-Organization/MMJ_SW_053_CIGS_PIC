#include "mmj_cigs_flash.h"                           // 同じフォルダのヘッダー
#include "../../system/mmj_cigs_config.h"             // システム設定
#include "../../../lib/device/mt25q.h"                // デバイスライブラリ

void misf_init()
{
    fprintf(PC, "MISSION FLASH Initialize\r\n");

    output_high(MIS_FM_CS);
    output_high(SMF_CS);
    delay_ms(100); 

    READ_ID_DATA read_id_data;
    int8 flash_cmd = CMD_READ_ID;
    output_low(mis_fm.cs_pin);
    spi_xfer_and_read_select_stream(mis_fm, &flash_cmd, 1, read_id_data.bytes, READ_ID_DATASIZE);
    output_high(mis_fm.cs_pin);
    fprintf(PC, "\t[MIS FM]READ ID:");
    for (unsigned int8 index = 0; index < 4; index++)
    {
        fprintf(PC, "%02X ", read_id_data.bytes[index]);
    }
    fprintf(PC,"\r\n");

    output_low(smf.cs_pin);
    spi_xfer_and_read_select_stream(smf, &flash_cmd, 1, read_id_data.bytes, READ_ID_DATASIZE);
    output_high(smf.cs_pin);
    fprintf(PC, "\t[SMF]\tREAD ID:");
    for (unsigned int8 index = 0; index < 4; index++)
    {
        fprintf(PC, "%02X ", read_id_data.bytes[index]);
    }
    fprintf(PC,"\r\n");

    if (!is_connect(smf)) {
        fprintf(PC, "\t[SMF]\tconnect error!\r\n");
        // return;
    }else {
        fprintf(PC, "\t[SMF]\tis connected\r\n");
    }
    delay_ms(100); // Wait for the flash to stabilize
    if (!is_connect(mis_fm)) {
        fprintf(PC, "\t[MIS FM]connect error!\r\n");
    }else {
        fprintf(PC, "\t[MIS FM]is connected\r\n");
    }

    unsigned int8 readdata[PACKET_SIZE];
    read_data_bytes(mis_fm, ADDRESS_MANEGE_START, readdata, PACKET_SIZE); // Read the PICLOG data header
    
    // Update the flash data header with the read data
    smf_piclog_use_counter = ((unsigned int32)readdata[0] << 24) | ((unsigned int32)readdata[1] << 16) | ((unsigned int32)readdata[2] << 8) | ((unsigned int32)readdata[3]);
    smf_piclog_loop_counter = readdata[4];
    smf_meas_use_counter = ((unsigned int32)readdata[5] << 24) |((unsigned int32)readdata[6] << 16) |((unsigned int32)readdata[7] << 8) | ((unsigned int32)readdata[8]);
    smf_meas_loop_counter = readdata[9];

    misf_piclog_use_counter = ((unsigned int32)readdata[10] << 24) |((unsigned int32)readdata[11] << 16) |((unsigned int32)readdata[12] << 8) | ((unsigned int32)readdata[13]);
    misf_piclog_loop_counter = readdata[14];
    misf_piclog_uncopyed_counter = ((unsigned int32)readdata[15] << 24) |((unsigned int32)readdata[16] << 16) |((unsigned int32)readdata[17] << 8) | ((unsigned int32)readdata[18]);
    misf_piclog_write_counter = ((unsigned int32)readdata[19] << 24) |((unsigned int32)readdata[20] << 16) |((unsigned int32)readdata[21] << 8) | ((unsigned int32)readdata[22]);
    misf_meas_use_counter = ((unsigned int32)readdata[23] << 24) |((unsigned int32)readdata[24] << 16) |((unsigned int32)readdata[25] << 8) | ((unsigned int32)readdata[26]);
    misf_meas_loop_counter = readdata[27];
    misf_meas_uncopyed_counter = ((unsigned int32)readdata[28] << 24) |((unsigned int32)readdata[29] << 16) |((unsigned int32)readdata[30] << 8) | ((unsigned int32)readdata[31]);

    fprintf(PC, "\t| SMF  | PICLOG | Use Counter      : 0x%08LX\r\n", smf_piclog_use_counter);
    fprintf(PC, "\t| SMF  | PICLOG | Loop Counter     : 0x%02X\r\n", smf_piclog_loop_counter);
    fprintf(PC, "\t| SMF  | MEAS   | Use Counter      : 0x%08LX\r\n", smf_meas_use_counter);
    fprintf(PC, "\t| SMF  | MEAS   | Loop Counter     : 0x%02X\r\n", smf_meas_loop_counter);
    fprintf(PC, "\t| MISF | PICLOG | Use Counter      : 0x%08LX\r\n", misf_piclog_use_counter);
    fprintf(PC, "\t| MISF | PICLOG | Uncopyed Counter : 0x%08LX\r\n", misf_piclog_uncopyed_counter);
    fprintf(PC, "\t| MISF | PICLOG | Loop Counter     : 0x%02X\r\n", misf_piclog_loop_counter);
    fprintf(PC, "\t| MISF | PICLOG | Write Counter    : 0x%02X\r\n", misf_piclog_write_counter);
    fprintf(PC, "\t| MISF | MEAS   | Use Counter      : 0x%08LX\r\n", misf_meas_use_counter);
    fprintf(PC, "\t| MISF | MEAS   | Uncopyed Counter : 0x%08LX\r\n", misf_meas_uncopyed_counter);
    fprintf(PC, "\t| MISF | MEAS   | Loop Counter     : 0x%02X\r\n", misf_meas_loop_counter);
    fprintf(PC, "\tComplete\r\n");
}

void smf_init()
{
   //Flash smf = {SPI_1, MT25QL01GBBB, SPI1_CS};
}

void write_misf_address_area()
{
    if (!is_connect(mis_fm)) {
        fprintf(PC, "Mission Flash is not connected\r\n");
        return;
    }
    //fprintf(PC, "Write MISF Address Area (Big Endian)\r\n");
    unsigned int8 writedata[PACKET_SIZE] = {0x00}; // Initialize write data buffer

    // 0〜3: smf_piclog_use_counter（MSB→LSB）
    writedata[0] = (smf_piclog_use_counter >> 24) & 0xFF;
    writedata[1] = (smf_piclog_use_counter >> 16) & 0xFF;
    writedata[2] = (smf_piclog_use_counter >> 8) & 0xFF;
    writedata[3] = smf_piclog_use_counter & 0xFF;

    // 4: smf_piclog_loop_counter
    writedata[4] = smf_piclog_loop_counter;

    // 5〜8: smf_meas_use_counter（MSB→LSB）
    writedata[5] = (smf_meas_use_counter >> 24) & 0xFF;
    writedata[6] = (smf_meas_use_counter >> 16) & 0xFF;
    writedata[7] = (smf_meas_use_counter >> 8) & 0xFF;
    writedata[8] = smf_meas_use_counter & 0xFF;

    // 9: smf_meas_loop_counter
    writedata[9] = smf_meas_loop_counter;

    // 10〜13: misf_piclog_use_counter（MSB→LSB）
    writedata[10] = (misf_piclog_use_counter >> 24) & 0xFF;
    writedata[11] = (misf_piclog_use_counter >> 16) & 0xFF;
    writedata[12] = (misf_piclog_use_counter >> 8) & 0xFF;
    writedata[13] = misf_piclog_use_counter & 0xFF;

    // 14: misf_piclog_loop_counter
    writedata[14] = misf_piclog_loop_counter;

    // 15〜18: misf_piclog_uncopyed_counter（MSB→LSB）
    writedata[15] = (misf_piclog_uncopyed_counter >> 24) & 0xFF;
    writedata[16] = (misf_piclog_uncopyed_counter >> 16) & 0xFF;
    writedata[17] = (misf_piclog_uncopyed_counter >> 8) & 0xFF;
    writedata[18] = misf_piclog_uncopyed_counter & 0xFF;

    // 19〜22: misf_piclog_write_counter（MSB→LSB）
    writedata[19] = (misf_piclog_write_counter >> 24) & 0xFF;
    writedata[20] = (misf_piclog_write_counter >> 16) & 0xFF;
    writedata[21] = (misf_piclog_write_counter >> 8) & 0xFF;
    writedata[22] = misf_piclog_write_counter & 0xFF;

    // 23〜26: misf_meas_use_counter（MSB→LSB）
    writedata[23] = (misf_meas_use_counter >> 24) & 0xFF;
    writedata[24] = (misf_meas_use_counter >> 16) & 0xFF;
    writedata[25] = (misf_meas_use_counter >> 8) & 0xFF;
    writedata[26] = misf_meas_use_counter & 0xFF;
    
    // 27: misf_meas_loop_counter
    writedata[27] = misf_meas_loop_counter;

    // 28〜31: misf_meas_uncopyed_counter（MSB→LSB）
    writedata[28] = (misf_meas_uncopyed_counter >> 24) & 0xFF;
    writedata[29] = (misf_meas_uncopyed_counter >> 16) & 0xFF;
    writedata[30] = (misf_meas_uncopyed_counter >> 8) & 0xFF;
    writedata[31] = misf_meas_uncopyed_counter & 0xFF;

    if (!is_connect(mis_fm)) {
        fprintf(PC, "Mission Flash is not connected\r\n");
        // return;
    }
    subsector_4kByte_erase(mis_fm, ADDRESS_MANEGE_START); // 4KBサブセクタを消去
    // Flash に書き込む
    write_data_bytes(mis_fm, ADDRESS_MANEGE_START, writedata, PACKET_SIZE);
    //fprintf(PC, "Write MISF Address Area Complete\r\n");

}

