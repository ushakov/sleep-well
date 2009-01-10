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
    mmcWrite(sector++, buf);
}

static int16_t get_num() {
    uint16_t n = 0;
    int16_t c;
    putCRLF(); putChar('#');
    do {
	while((c = uart_getchar()) == -1);
	if (c == '\r') return n;
	if (c >= '0' && c <= '9') {
	    n *= 10;
	    n += c - '0';
	} else {
	    putChar('?'); putCRLF();
	    n = 0;
	}
    } while (1);
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

static void monitor() {
    putProg("Welcome to Monitor!\r\n");
    while(1) {
	putChar('>');
	int16_t c;
	while ((c = uart_getchar()) == -1) ;
	switch(c) {
	case 't':  // get time
	    putHexF(time_get(), 0);
	    putCRLF();
	    break;
	case 'd':  // Dump all
	    for (uint32_t s = 0; s < sector; ++s) {
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
	    break;
	default:
	    putChar('?');
	    putCRLF();
	}
    }
}

void main() {
    time_init();
    time_set(0);
    mmcInit();
    accel_init();
    accel_set_param();
    uart_init();
    uart_set_baud_rate(38400);

    sector = 0;
    blen = 0;

    seq = eeprom_read(0);
    seq++;

    eeprom_write(0, seq);

    putProg("Welcome to logger!\r\n");
    if (mmcReset() != 0) {
	delay_s(1);
	putProg("retrying...");
	if (mmcReset() != 0) {
	    putProg("bad luck");
	}
    }

//    dump_some_mem(); while(1);
    
    write_header();
    int16_t min, max;
    while(1) {
	if (uart_getchar() != -1) {
	    putProg("-->\r\n");
	    monitor();
	}
	if (blen == 512) {
	    putProg("dumping: sector=");
	    putHexF(sector,0);
	    putProg(" seq=");
	    putHexF(seq,0);
	    putProg(" min=");
	    putHexF(min,0);
	    putProg(" max=");
	    putHexF(max,0);
	    putCRLF();

	    dump_buffer();
	    write_header();
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
	    if (acc == -2) {
		putProg("bad device\r\n");
	    } else if (acc == -1) {
		putProg("Gave up\r\n");
	    } else {
		putProg("unknown: ");
		putHexF(acc,2);
		putCRLF();
	    }
	}
    }
}
