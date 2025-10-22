#ifndef VALUE_STATUS_H
#define VALUE_STATUS_H

#include "../tool/mmj_smf_memorymap.h"

volatile int1 is_use_smf_req_in_mission = FALSE;
volatile int8 status[4] = {IDLE, DATA_ID_CIGS_PICLOG, DATA_ID_CIGS_IV1_DATA, DATA_ID_CIGS_IV2_DATA};


#endif

