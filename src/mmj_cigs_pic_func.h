#ifndef MMJ_CIGS_PIC_FUNC_H
#define MMJ_CIGS_PIC_FUNC_H

// _________ values _________
#define MISSION_DATA_SIZE 64
volatile unsigned int8 mission_datas[MISSION_DATA_SIZE] = {0x00};
volatile int8 executed_mission_count = 0;

#endif
