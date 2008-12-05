#include "manchester.h"
#include "delay.h"

#include "config.h"

void manchester_init() {
#ifdef SENSOR
    DDR_RADIO |= (1 << BIT_RADIO);
#else  // SENSOR
    DDR_RADIO &= ~(1 << BIT_RADIO);
    PORT_RADIO &= ~(1 << BIT_RADIO);
#endif
}

// in microseconds
#define HALF_BIT 250
void manchester_send_bit(int b) {
    if (b) {
	PORT_RADIO &= ~(1 << BIT_RADIO);
	delay_us(HALF_BIT);
	PORT_RADIO |= (1 << BIT_RADIO);
	delay_us(HALF_BIT);
    } else {
	PORT_RADIO |= (1 << BIT_RADIO);
	delay_us(HALF_BIT);
	PORT_RADIO &= ~(1 << BIT_RADIO);
	delay_us(HALF_BIT);
    }
}

int manchester_wait_bit() {
    // wait for large part of the half-bit
    delay_us(3*HALF_BIT/4);
    int first_part = 0;
    // wait until we get a good sample
    do {
	first_part = 0;
	for(int i = 0; i < 10; ++i) {
	    if (PIN_RADIO & (1 << BIT_RADIO)) {
		first_part ++;
	    }
	}
    } while (first_part > 2 && first_part < 8);
    int expected_second = (first_part < 5);
    int got_second = 0;
    while (got_second < 4) {
	if (expected_second && (PIN_RADIO & (1 << BIT_RADIO)) ||
	    !expected_second && ((PIN_RADIO & (1 << BIT_RADIO)) == 0)) {
	    got_second++;
	} else {
	    got_second = 0;
	}
    }
    delay_us(3*HALF_BIT/4);
    return expected_second;
}

// Send 10-bit number in Manchester code.
void manchester_send(int t) {
    int mask = 0x200;
    manchester_send_bit(1);
    manchester_send_bit(1);
    manchester_send_bit(1);
    manchester_send_bit(0);
    int ones = 0;
    for (int i = 0; i < 10; ++i) {
	int bit =  !! (t & mask);
	manchester_send_bit(bit);
	mask >>= 1;
	if (bit == 1) {
	    ones ++;
	} else {
	    ones = 0;
	}
	if (ones == 2) {
	    manchester_send_bit(0);
	    ones = 0;
	}
    }
    if (ones > 0) {
	manchester_send_bit(0);
    }
}

int manchester_try_read() {
    // wait for sync
    int ones = 0;
    int bit;
    while (ones != 3) {
	bit = manchester_wait_bit();
	if (bit) {
	    ones++;
	} else {
	    ones = 0;
	}
    }
    bit = manchester_wait_bit();
    if (bit) {
	// failed...
	return -1;
    }
    ones = 0;
    int ret = 0;
    for (int i = 0; i < 10; ++i) {
	bit = manchester_wait_bit();
	ret |= bit;
	if (bit == 1) {
	    ones ++;
	} else {
	    ones = 0;
	}
	if (ones == 2) {
	    bit = manchester_wait_bit();
	    if (bit) {
		// failed...
		return -1;
	    }
	}
    }
    return ret;
}

int manchester_read() {
    int t;
    while ((t = manchester_try_read()) < 0);
    return t;
}
