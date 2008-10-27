#include "uart.h"

main() {
    uart_init();
    uart_set_baud_rate(2400);
    while(1) {
	for (int i = 0; i < 80; ++i) {
	    uart_write_byte('+');
	}
	uart_write_byte('\r');
	uart_write_byte('\n');
    }
}
