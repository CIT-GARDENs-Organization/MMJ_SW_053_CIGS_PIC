#ifndef COMMUNICATION_DRIVER_H
#define COMMUNICATION_DRIVER_H

#include "communication.h"


// This is redundant, but it is a process to hide the Communication library as a driver.
Command make_command(unsigned int8 receive_signal[], int8 receive_signal_size);

void transmit_ack();

void transmit_status();

int1 req_use_smf();


#include "./src/communication_driver.c"

#endif

// EOF
