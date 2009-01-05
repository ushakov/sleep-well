#include "mmc.h"
#include "accel.h"
#include "uart.h"
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
    accel_init();
    accel_set_param();
    sector = 0;
    blen = 0;
    seq = eeprom_read(0);
    seq++;
    eeprom_write(0, seq);
    while(1) {
	if (blen == 512) {
	    dump_buffer();
	    write_header();
	}
	int16_t acc = accel_get_overall();
	if (acc >= 0) {
	    buf[blen++] = acc & 0xff;
	    buf[blen++] = (acc >> 8) & 0xff;
	}
    }
}
