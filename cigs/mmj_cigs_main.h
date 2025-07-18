#ifndef MMJ_CIGS_MAIN_H
#define MMJ_CIGS_MAIN_H

//#opt 0 // 0 = no optimization, 1 = optimize for speed, 2 = optimize for size

// pin assignment etc
#include "mmj_cigs_config.h"

#include "../lib/communication/typedef_content.h"

#include "../lib/communication/value_status.h"

#include "../lib/tool/timer.h"

#include "../lib/communication/uart.h"

// SerialCommunication
#include "../lib/communication/communication_driver.h"

#include "../lib/tool/smf_function.h"




#include "mmj_cigs_excute_mission.h"

#include "mmj_cigs_piclog.h"

#include "mmj_cigs_func.h"

#include "mmj_cigs_flash.h"


#endif // MMJ_CIGS_MAIN_H
//------------------End of File------------------
