#include <avr/io.h>
#include <inttypes.h>

#include "delay.h"
#include "uart.h"
#include "put.h"
#include "mmc.h"

uint8_t buf[512];

void flash_led() {
    PORTD |= _BV(4);
    delay_ms(200);
    PORTD &= ~_BV(4);
    delay_ms(200);
}

void main() {
    uart_init();
    uart_set_baud_rate(38400);
    mmcInit();

    putProg("SD test starting!\r\n");
    if (mmcReset() != 0) {
	delay_s(1);
	putProg("retrying...");
	if (mmcReset() != 0) {
	    putProg("bad luck");
	    while(1);
	}
    }
    for(int i = 0; i < 512; ++i) {
	buf[i] = i % 256;
    }
    if (mmcWrite(1000, buf) != 0) while(1);
    if (mmcRead(1000, buf) != 0) while(1);
    for (int i = 0; i < 512; ++i) {
	if (i % 32 == 0) {
	    putHexF(i, 4);
	    putProg(" ");
	}
	putHexF(buf[i], 2);
	putProg(" ");
	if (i % 32 == 31) {
	    putCRLF();
	}
    }
    while(1);
}

