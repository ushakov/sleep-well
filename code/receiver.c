#include "config.h"

#include "delay.h"
#include "uart.h"
#include "manchester.h"

#define BUFLEN 50
static uint16_t buffer[BUFLEN];

uint8_t hexdigit(uint8_t n) {
    if (n < 10) {
	return '0' + n;
    } else {
	return 'a' + n - 10;
    }
}

void hex(uint16_t n) {
    uart_write_byte(hexdigit((n >> 8) & 0xf));
    uart_write_byte(hexdigit((n >> 4) & 0xf));
    uart_write_byte(hexdigit(n & 0xf));
}

void crlf() {
    uart_write_byte('\r');
    uart_write_byte('\n');
}

void main() {
    manchester_init();
    uart_init();
    uart_set_baud_rate(19200);
    delay_s(1);
    for (int i = 0; i < BUFLEN; ++i) {
	uint16_t val = manchester_read();
	hex(val);
	uart_write_byte('\r');
    }
    while(1);
}
