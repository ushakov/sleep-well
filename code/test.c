#include <avr/io.h>

#include "uart.h"

void printout(int d) {
    int t = 1000;
    while (t != 0) {
	uart_write_byte(d / t + '0');
	d %= t;
	t /= 10;
    }
    uart_write_byte('\r');
    uart_write_byte('\n');
}

main() {
    uart_init();
    uart_set_baud_rate(115200);
    DDRA = 0;
    PORTA = 0;
    ADMUX = 0xc7; // 1100 0111
    ADCSRA = 0x87;  // 1000 0111
    while(1) {
	int c = 0;
	ADCSRA |= 0x40;
	while (ADCSRA & 0x40) c++;
	int t = ADCL;
	t |= ADCH  << 8;
	printout(t);
	uart_write_byte('>'); printout(c);
	for (uint32_t k = 0; k < 100000UL; ++k) { __asm__ ( "nop" ); }
    }
}
