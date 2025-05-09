#ifndef MISSION_TOOLS_H
#define MISSION_TOOLS_H

#include "communication_driver.h"


// _____________ define _______________

#define EXECUTED_MISSION_SIZE 8


// _____________ typedef ________________

typedef struct {
   unsigned int8 executed_mission[EXECUTED_MISSION_SIZE];
   int8 executed_mission_count;
} ExecutedMission;


// ________________ fuctions ____________

// Value management using the Singleton pattern
ExecutedMission get_executed_mission();

int1 is_executed_mission(unsigned int8 command_id);

void add_executed_mission(unsigned int8 command_id);

// Use in missions you want to execute consecutively
void executed_mission_pop(void);



// Check and reply to messages received from BOSS PIC
void check_and_respond_to_boss();

// MIS MCU will be up and running duration_second seconds after mission completion
void continue_mis_mcu(int16 duration_second);



// Request using SMF to BOSS PIC
// !!!After use, you must call `void finished_use_smf()`
int1 req_use_smf();

// Call after using SMF
void finished_use_smf();


#include "./src/mission_tools.c"

#endif
//------------------End of File------------------