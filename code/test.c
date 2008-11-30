#include <avr/io.h>
#include "delay.h"

#define DDR_LED DDRA
#define PORT_LED PORTA
#define BIT_LED PORTA1

#define DDR_TR DDRA
#define PORT_TR PORTA
#define BIT_TR PORTA2

#define DDR_RADIO DDRA
#define PORT_RADIO PORTA
#define BIT_RADIO PORTA7

void init() {
    DDR_LED |= BIT_LED;
    PORT_LED |= BIT_LED;

    DDR_RADIO |= BIT_RADIO;
}

void main() {
    init();
    while (1) {
	delay_ms(5);
	PORT_RADIO |= BIT_RADIO;
	delay_ms(5);
	PORT_RADIO &= ~BIT_RADIO;
    }
//    ADMUX = 0xc1; // 1100 0001, single +ADC1
//    ADMUX = 0xd0; // 1101 0000, diff +ADC0, -ADC1, x1
//    ADMUX = 0xc0; // 1100 0000, single +ADC0
//    ADMUX = 0xc9; // 1100 1001, diff -ADC0, +ADC1, x10
//    ADCSRA = 0x87;  // 1000 0111
/*     while(1) { */
/* 	ADCSRA |= 0x40; */
/* 	while (ADCSRA & 0x40); */
/* 	int t = ADCL; */
/* 	t |= ADCH  << 8; */
/* 	printout(t); */
/*     } */
}
