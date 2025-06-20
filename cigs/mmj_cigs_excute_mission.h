#ifndef MMJ_CIGS_EXCUTE_MISSION_H
#define MMJ_CIGS_EXCUTE_MISSION_H

// _________ values _________
//#define PARAMETER_LENGTH 8

#Separate
int1 execute_command(Command *command);
#Separate
void handle_uplink_command(Command *command);
#Separate
int1 handle_status_check(Command *command);
#Separate
void handle_smf_available(Command *command);


#separate
void execute_mission(unsigned int8 *content);

unsigned int8 duration_sec = 30; // duration sec for mission

#include "src/mmj_cigs_excute_mission.c"

#endif
//------------------End of File------------------