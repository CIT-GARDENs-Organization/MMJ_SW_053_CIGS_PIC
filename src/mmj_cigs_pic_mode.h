#ifndef MMJ_CIGS_PIC_MODE_H
#define MMJ_CIGS_PIC_MODE_H

// ___________ include _____________
# include <mmj_cigs_pic_func.h>

// _________ values _________
#define EXECUTED_MISSION_SIZE 64
volatile unsigned int8 executed_mission[EXECUTED_MISSION_SIZE] = {0x00};
volatile int8 executed_mission_count = 0;

#define PARAMETER_LENGTH 8


// __________ functions ____________

// fundamental fnction
void execute_mission(unsigned int8 *content);

// missions
void example_00(unsigned int8 parameter[]);
void example_01(unsigned int8 parameter[]);

// other tools
void executed_mission_pop(void);
void enqueue_smf_data(unsigned int32 src, unsigned int32 dest, unsigned int32 size);
void update_time(unsigned int8 raw_time[]);

volatile unsigned int array[300] = {0}; 


#endif
