#include "cigs_piclog.h"                          // 同じフォルダのヘッダー
#include "../hal/timer.h"                   // タイマーライブラリ  
#include "cigs_flash.h"                 // ストレージ機能
#include "../lib/tool/smf_queue.h"               // フラッシュ操作キュー
#include "../lib/device/mt25q.h"                 // フラッシュデバイス

#define MISSION_ID_PICLOG 0x03  // PICLOGミッションID

void piclog_save(int8 function, int8 parameter)
{
    // Initialize PICLOG data structure
    piclog_t piclog;
    piclog_t *piclog_ptr = &piclog;
    memset(piclog_ptr, 0x00, PICLOG_PACKET_SIZE);
    
    // Fill PICLOG fields
    piclog_ptr->fields.time = get_current_sec();
    piclog_ptr->fields.function = function;
    piclog_ptr->fields.parameter = parameter;


    printf("[PICLOG] : ");
    for (int8 i = 0; i < PICLOG_PACKET_SIZE; i++) {
        printf("%02X ", piclog_ptr->bytes[i]);
    }
    printf("\r\n");

    
    misf_write_data(FLASH_ID_PICLOG, piclog_ptr->bytes, PICLOG_PACKET_SIZE);
    misf_update_address_area();
}
// End of file