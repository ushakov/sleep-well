#include "config.h"

#include "manchester.h"

void init() {
    DDR_LED |= _BV(BIT_LED);
}

void init_adc() {
    ADMUX = 0x81; // 1000 0001, single +ADC1
    ADCSRA = 0x86;  // 1000 0110
    ADCSRB = 0x0;
    DIDR0 = ~0x2; // disable digital input on ADC1
}

void main() {
    init();
    manchester_init();
    init_adc();
    delay_s(1);
    PORT_LED &= ~_BV(BIT_LED);
    while(1) {
 	ADCSRA |= 0x40;
	while (ADCSRA & 0x40);
	int16_t t = ADCL;
	t |= (ADCH << 8);
	manchester_send(t);
    }

    while(1);
}
