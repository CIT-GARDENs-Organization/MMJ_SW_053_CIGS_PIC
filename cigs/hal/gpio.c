#include "mmj_cigs_config.h"
#include "gpio.h"
#include "../../lib/tool/types.h"


void gpio_init() {
    enable_negative_power();
    disconnect_port1();
    disconnect_port2();
}

void enable_negative_power() {
    output_low(EN_NPWR1);
}

void connect_negative_power1(int1 state) {
    if (state) {
        output_low(EN_NPWR1);
    } else {
        output_high(EN_NPWR1);
    }
}

void connect_negative_power2(int1 state) {
    if (state) {
        output_high(EN_NPWR2);
    } else {
        output_low(EN_NPWR2);
    }
}

void connect_port1(){
    output_high(CONNECT_CIGS1);
}

void disconnect_port1(){
    output_low(CONNECT_CIGS1);
}

void connect_port2(){
    output_high(CONNECT_CIGS2);
}

void disconnect_port2(){
    output_low(CONNECT_CIGS2);
}

