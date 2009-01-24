#include "mmc.h"
#include "accel.h"
#include "uart.h"
#include "put.h"
#include "time.h"
#include "eeprom.h"
#include "delay.h"

uint8_t buf[512];
uint16_t blen;
uint32_t sector;

uint8_t seq;

static void led_init() {
    DDRC |= _BV(2);
}

static void led_on() {
    PORTC |= _BV(2);
}

static void led_off() {
    PORTC &= ~_BV(2);
}

static void write_header() {
    blen = 0;

    // sequence number, 1 byte
    buf[blen++] = seq++;

    // time at start, 4 bytes
    uint32_t t = time_get();
    buf[blen++] = (t) & 0xff;
    buf[blen++] = (t >> 8) & 0xff;
    buf[blen++] = (t >> 16) & 0xff;
    buf[blen++] = (t >> 24) & 0xff;

    // padding (reserved), 3 bytes
    buf[blen++] = 0;
    buf[blen++] = 0;
    buf[blen++] = 0;

    // total: 8 bytes header
}

static void dump_buffer() {
    uint8_t r = mmcWrite(sector++, buf);
    if (r == 0) {
	while(1) {
	    led_on();
	    delay_ms(200);
	    led_off();
	    delay_ms(100);
	}
	while(1);
    }
}

static void dump_some_mem() {
    for (uint32_t s = 0; s < 100; ++s) {
	int8_t r1 = mmcRead(s, buf);
	if (r1 != 0) {
	    putProg("Bad read sector=0x");
	    putHexF(s, 2);
	    putProg(" res=0x");
	    putHexF(r1, 2);
	    putCRLF();
	    break;
	}
	for (int16_t i = 0; i < 512; ++i) {
	    putHexF(buf[i], 2);
	    if(i % 16 == 15) {
		putCRLF();
	    } else {
		putChar(' ');
	    }
	}
	putCRLF();
    }
}

void main() {
    DDRB = 0xff;
    DDRC = 0xff;
    DDRD = 0xff;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    time_init();
    time_set(0);
    mmcInit();
    accel_init();
    accel_set_param();
//    uart_init();
//    uart_set_baud_rate(38400);
    led_init();
    led_on();

    sector = 0;
    blen = 0;

    seq = eeprom_read(0);
    seq++;

    eeprom_write(0, seq);

//    putProg("Welcome to logger!\r\n");
    if (mmcReset() != 0) {
	delay_s(1);
//	putProg("retrying...");
	if (mmcReset() != 0) {
//	    putProg("bad luck");
	    while(1);
	}
    }
    led_off();

//    dump_some_mem(); while(1);
    
    write_header();
    int16_t min, max;
    while(1) {
	if (blen == 512) {
#ifdef UART_DUMP
	    putProg("dumping: sector=");
	    putHexF(sector,0);
	    putProg(" seq=");
	    putHexF(seq,0);
	    putProg(" min=");
	    putHexF(min,0);
	    putProg(" max=");
	    putHexF(max,0);
	    putCRLF();
#endif  // UART_DUMP

	    dump_buffer();
	    write_header();
	    led_on();
	}
	if (blen == 28) {
	    led_off();
	}
	int16_t acc = accel_get_overall();
	if (acc >= 0) {
	    if (blen == 8) {
		max = acc;
		min = acc;
	    } else {
		if (min > acc) min = acc;
		if (max < acc) max = acc;
	    }
	    buf[blen++] = acc & 0xff;
	    buf[blen++] = (acc >> 8) & 0xff;
	} else {
	    led_on();
	    delay_ms(100);
	    led_off();
	    delay_ms(100);
#ifdef UART_DUMP
	    if (acc == -2) {
		putProg("bad device\r\n");
	    } else if (acc == -1) {
		putProg("Gave up\r\n");
	    } else {
		putProg("unknown: ");
		putHexF(acc,2);
		putCRLF();
	    }
#endif  // UART_DUMP
	}
    }
}
