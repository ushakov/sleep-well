#include "config.h"

#include "manchester.h"

/* void main() { */
/*     init(); */
/*     manchester_init(); */
/*     init_adc(); */
/*     while (1) { */
/*  	ADCSRA |= 0x40; */
/* 	while (ADCSRA & 0x40); */
/* 	int t = ADCL; */
/* 	t |= ADCH << 8; */
/* 	int t = 57; */
/* 	manchester_send(t); */
/* 	PORT_LED |= (1 << BIT_LED); */
/* 	delay_ms(1); */
/* 	PORT_LED &= ~(1 << BIT_LED);  */
/* 	delay_ms(1); */
/*     } */
/* } */


void main() {
    manchester_init();
    while (1) {
	manchester_send_bit(0);
	manchester_send_bit(1);
	manchester_send_bit(0);
	manchester_send_bit(1);
	manchester_send_bit(1);
    }
}
