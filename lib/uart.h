#ifndef UART_H
#define UART_H

// _________ defines ________________

#define RECEIVE_BUFFER_MAX 32


// _____________ values _______________

volatile unsigned int8 boss_receive_buffer[RECEIVE_BUFFER_MAX] = {0x00};
volatile int8 boss_receive_buffer_size = 0;


// _______________ functions ___________

#INT_RDA
static void RDA_isr(void);

void setup_uart_to_boss();

void clear_receive_signal(unsigned int8 receive_signal[], int8 *receive_signal_size);


#include "./src/uart.c"

#endif

