#include "mmc.h"
#include "accel.h"
#include "uart.h"
#include "put.h"
#include "time.h"
#include "eeprom.h"

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

    write_header();
    int16_t min, max;
    // putProg("1\r\n");
    while(1) {
	if (blen == 512) {
	    putProg("dumping:\r\nSector=");
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
	// putProg("2\r\n");
	int16_t acc = accel_get_overall();
	// putProg("3: ");
	// putHexF(acc,2);
	// putCRLF();
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
	    if (0) {
		putProg("CP: blen=0x");
		putHexF(blen,2);
		putCRLF();
	    }
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
