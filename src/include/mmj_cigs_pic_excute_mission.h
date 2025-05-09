#ifndef MMJ_CIGS_PIC_EXCUTE_MISSION_H
#define MMJ_CIGS_PIC_EXCUTE_MISSION_H

// _________ values _________
#define PARAMETER_LENGTH 8

#Separate
int1 execute_command(Command *command);
int1 handle_uplink_command(Command *command);
int1 handle_status_check(Command *command);
int1 handle_smf_available(Command *command);
void execute_mission(unsigned int8 *content);

#include "../mmj_cigs_pic_excute_mission.c"

#endif
//------------------End of File------------------