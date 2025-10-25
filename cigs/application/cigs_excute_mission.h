#ifndef MMJ_CIGS_EXCUTE_MISSION_H
#define MMJ_CIGS_EXCUTE_MISSION_H

// _________ values _________
//#define PARAMETER_LENGTH 8
// #include "../lib/communication/mission_tools.h"



int1 execute_command(Command *command);

void handle_uplink_command(Command *command);

int1 handle_status_check(Command *command);

void handle_smf_available(Command *command);

void handle_recieve_time(Command *command);


void execute_mission(int8 *content);

int8 duration_sec = 30; // duration sec for mission


#endif
//------------------End of File------------------