#include "mmj_cigs_piclog.h"                          // 同じフォルダのヘッダー
#include "../../hardware/mcu/timer.h"                   // タイマーライブラリ  
#include "../storage/mmj_cigs_flash.h"                 // ストレージ機能
#include "../../../lib/tool/smf_queue.h"               // フラッシュ操作キュー
#include "../../../lib/device/mt25q.h"                 // フラッシュデバイス

// #define PICLOG_DEBUG
#define MISSION_ID_PICLOG 0x03  // PICLOGミッションID

void piclog_make(int8 function, int8 parameter)
{
    PICLOG_t piclog;
    memset(piclog.bytes, 0, PICLOG_PACKET_SIZE);
    piclog.fields.time = get_current_sec();
    piclog.fields.function = function;
    piclog.fields.parameter = parameter;

    printf("\t[PICLOG] : ");
    for (int8 i = 0; i < PICLOG_PACKET_SIZE; i++) {
        printf("%02X ", piclog.bytes[i]);
    }
    printf("\r\n");

    int32 write_address = MISF_CIGS_PICLOG_START + piclog_data.used_counter;
    write_data_bytes(mis_fm, write_address, piclog.bytes, PICLOG_PACKET_SIZE);

    piclog_data.used_counter += PICLOG_PACKET_SIZE;
    piclog_data.uncopied_counter += PICLOG_PACKET_SIZE;
    piclog_data.reserve_counter1 += PICLOG_PACKET_SIZE;

    // Next Packet
    if (piclog_data.reserve_counter1 + PICLOG_PACKET_SIZE >=  PACKET_SIZE) {
        write_address = MISF_CIGS_PICLOG_START + piclog_data.used_counter;
        write_data_bytes(mis_fm, write_address, PICLOG_BLANK_DATA, sizeof(PICLOG_BLANK_DATA));
        piclog_data.used_counter += sizeof(PICLOG_BLANK_DATA);
        piclog_data.uncopied_counter += sizeof(PICLOG_BLANK_DATA);
        piclog_data.reserve_counter1 = 0;
    }

    write_misf_address_area(); // Update the address area after writing
    
    /*
    // Add CRC Check
    if (misf_piclog_write_counter >= MISF_PICLOG_MAX_COUNT) {
        write_address = ADDRESS_MISF_PICLOG_DATA_START + misf_piclog_use_counter;
        write_data_bytes(mis_fm, write_address, *PICLOG_BLANK_DATA, PICLOG_BLANK_SIZE);
        unsigned int8 piclog_data_header[PICLOG_BLANK_SIZE] = {0x00, 0x00, 0x00}; // Initialize blank data
        misf_piclog_write_counter = 0; // Reset if max count reached
    }
    */
    #ifdef PICLOG_DEBUG
        //fprintf(PC, "PICLOG written: %02X %02X %02X %02X %02X %02X\r\n", piclog_data.bytes[0], data[1], data[2], data[3], data[4], data[5]);
    #endif
}
// End of file