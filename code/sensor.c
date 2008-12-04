#include "config.h"

void init() {
    DDR_LED |= (1 << BIT_LED);
    PORT_LED |= (1 << BIT_LED);
}

void init_adc() {
    ADMUX = 0x81; // 1000 0001, single +ADC1
    ADCSRA = 0x86;  // 1000 0110
    ADCSRB = 0x0;
    DIDR0 = ~0x2; // disable digital input on ADC1
}

void main() {
    init();
    // init_adc();
    
    // Turn on LED
    // PORT_LED &= ~(1 << BIT_LED);

    while (1) {
/*  	ADCSRA |= 0x40; */
/* 	while (ADCSRA & 0x40); */
/* 	int t = ADCL; */
/* 	t |= ADCH << 8; */
	int t = 57;
	manchester_send(t);
    }
}