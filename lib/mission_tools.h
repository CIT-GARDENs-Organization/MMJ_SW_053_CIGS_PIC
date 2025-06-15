#ifndef MISSION_TOOLS_H
#define MISSION_TOOLS_H

#include "communication_driver.h"


// ________________ fuctions ____________

// Check and reply to messages received from BOSS PIC
void check_and_respond_to_boss();

// Request using SMF to BOSS PIC
// !!!After use, you must call `void finished_use_smf()`
int1 req_use_smf();

// Call after using SMF
void finished_use_smf();

//unsigned int8 calc_crc8(unsigned int8 frame[], int8 payload_size);

#include "./src/mission_tools.c"

#endif

