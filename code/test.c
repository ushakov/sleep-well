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
    ADMUX = 0xc7; // 1100 0111
    ADCSRA = 0x87;  // 1000 0111
    while(1) {
	ADCSRA |= 0x40;
	while (ADCSRA & 0x40) ;
	int t = ADCH << 8;
	t |= ADCL;
	printout(t);
    }
}
