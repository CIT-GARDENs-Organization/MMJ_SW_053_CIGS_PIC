#ifndef MMJ_CIGS_MAIN_H
#define MMJ_CIGS_MAIN_H

//#opt 0 // 0 = no optimization, 1 = optimize for speed, 2 = optimize for size

// pin assignment etc
#include "mmj_cigs_config.h"

#include "../lib/typedef_content.h"

#include "../lib/value_status.h"

#include "../lib/timer.h"

#include "../lib/uart.h"

// Missions
#include "mmj_cigs_mode.h"

// SerialCommunication
#include "../lib/communication_driver.h"

// Copy to SMF
#include "../lib/data_copy.h"

#include "mmj_cigs_excute_mission.h"

#include "mmj_cigs_piclog.h"

#include "mmj_cigs_func.h"

#include "mmj_cigs_flash.h"

#include "lib/ad7490.h"

#endif // MMJ_CIGS_MAIN_H
//------------------End of File------------------
