#include "config.h"

#include "uart.h"
#include "manchester.h"

#define BUFLEN 100
static uint8_t buffer[BUFLEN];

void main() {
    manchester_init();
    delay_s(1);
    for (int i = 0; i < BUFLEN; ++i) {
	buffer[i] = manchester_wait_bit();
    }
    uart_init();
    uart_set_baud_rate(19200);
    for(int i = 0; i < BUFLEN; ++i) {
	uart_write_byte(buffer[i]);
    }
    uart_write_byte('\r');
    uart_write_byte('\n');
    while(1);
}
