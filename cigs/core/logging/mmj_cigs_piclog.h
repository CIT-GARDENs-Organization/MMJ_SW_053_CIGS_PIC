#ifndef     MMJ_CIGS_PICLOG_H
#define     MMJ_CIGS_PICLOG_H

#define PICLOG_PACKET_SIZE 6
unsigned int8 PICLOG_STARTUP = 0x00;

// PICLOG Parameter
#define PICLOG_PARAM_START 0x01
#define PICLOG_PARAM_END 0x02


// PICLOG
typedef union {
    struct {
        unsigned int32  time;
        unsigned int8   function;
        unsigned int8   parameter;
    };
    unsigned int8 bytes[PICLOG_PACKET_SIZE];  
} PICLOG;

// extern宣言を追加（実体は.cファイルで定義）
#define PICLOG_BLANK_SIZE 4 // PICLOGの空データサイズ
unsigned int8 PICLOG_BLANK_DATA[PICLOG_BLANK_SIZE] = {0x00, 0x00, 0x00, 0x00}; // PICLOGの空データ
void piclog_make(unsigned int8 function, unsigned int8 parameter);
void piclog_write(unsigned int8 *data);





#endif // MMJ_CIGS_PICLOG_H
//------------------End of File------------------
