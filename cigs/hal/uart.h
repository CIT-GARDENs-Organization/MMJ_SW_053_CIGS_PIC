#ifndef _UART_H_
#define _UART_H_

// _________ defines ________________

#define RECEIVE_BUFFER_MAX 32


// _____________ values _______________

volatile unsigned int8 boss_receive_buffer[RECEIVE_BUFFER_MAX] = {0x00};
volatile int8 boss_receive_buffer_size = 0;


// _______________ functions ___________

#INT_RDA3
static void boss_receive_isr(void);

void setup_uart_to_boss();

void clear_receive_signal(unsigned int8 receive_signal[], int8 *receive_signal_size);



#endif // _UART_H_
//------------------End of File------------------