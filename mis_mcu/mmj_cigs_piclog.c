#include "include/mmj_cigs_flash.h"
#include "include/mmj_cigs_piclog.h"


PICLOG piclog_data;

void piclog_init()
{
    //PICLOG piclog_data = {0}; // Initialize PICLOG structure

}

void piclog_make(unsigned int8 function, unsigned int8 parameter)
{
    piclog_init(); // 初期化

    // 時刻取得関数がある場合はそれを使う
    // 例: get_current_time_array(piclog_data.time);
    for (int i = 0; i < PICLOG_TIME_SIZE; i++) {
        piclog_data.time[i] = 0; // 仮の初期化
    }
    piclog_data.function = function;
    piclog_data.parameter = parameter;

    // フラッシュ書き込み
    piclog_write((unsigned int8 *)&piclog_data, PICLOG_TOTAL_SIZE);
    fprintf(PC, "PICLOG data written: Function %d, Parameter %d\r\n", function, parameter);
}

void piclog_write()
{
    // Assuming a function to write data to flash
    //write_data_bytes(MIS_FM_STREAM, ADDRESS_PICLOG_START, data, size);
    
    fprintf(PC, "PICLOG data written successfully\r\n");
}
