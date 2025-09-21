#ifndef GPIO_H
#define GPIO_H

void gpio_init();
void enable_negative_power();
void disable_negative_power();
void connect_port1();
void disconnect_port1();
void connect_port2();
void disconnect_port2();


// 
void connect_negative_power1(int1 state);
void connect_negative_power2(int1 state);
#endif // GPIO_H
