#include <avr/io.h>
#include <inttypes.h>

#include "eeprom.h"
#include "accel.h"
#include "delay.h"
#include "uart.h"
#include "put.h"

uint8_t buf[64];

void flash_led() {
    PORTD |= _BV(4);
    delay_ms(200);
    PORTD &= ~_BV(4);
    delay_ms(200);
}

void main() {
    accel_init();
    DDRD |= _BV(4);
    PORTD &= ~_BV(4);
    uart_init();
    uart_set_baud_rate(38400);

    putProg("Accel starting!\r\n");
    flash_led();
//    accel_write(0x20, 0xe7);
    accel_write(0x20, 0xc7);
    delay_s(2);
    while(1) {
	int16_t i = 0;
	int16_t hm = 0;
	if (accel_read(0x0f) != 0x3a) {
	    putProg("bad device\r\n");
	    while(accel_read(0x0f) != 0x3a);
	}
	while ((accel_read(0x27) & 0x4) == 0) {
	    i++;
	    if(i == 100) {
		accel_write(0x20, 0xc7);
	    }
	}
	int16_t x = accel_read(0x28);
	x |= (accel_read(0x29) << 8);
	int16_t y = accel_read(0x2a);
	y |= (accel_read(0x2b) << 8);
	int16_t z = accel_read(0x2c);
	z |= (accel_read(0x2d) << 8);
	putHexF(x,2);
	putChar(' ');
	putHexF(y,2);
	putChar(' ');
	putHexF(z,2);
	putChar(' ');
	putCRLF();
    }
    while(1);
}

