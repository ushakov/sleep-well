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
void manchester_send_bit(int8_t b) {
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

int8_t current_bit() {
    if (PIN_RADIO & (1 << BIT_RADIO)) {
	return 1;
    } else {
	return 0;
    }
}

// Send 10-bit number in Manchester code.
void manchester_send(uint16_t t) {
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

int8_t manchester_wait_bit() {
    // wait for large part of the half-bit
    delay_us(3*HALF_BIT/4);
    int8_t first_part = 0;
    // wait until we get a good sample
    do {
	first_part = 0;
	for(int8_t i = 0; i < 10; ++i) {
	    first_part += current_bit();
	}
    } while (first_part > 2 && first_part < 8);
    int8_t expected_second = (first_part < 5) ? 1 : 0;
    int8_t got_second = 0;
    while (got_second < 4) {
	if (expected_second == current_bit()) {
	    got_second++;
	} else {
	    got_second = 0;
	}
    }
    delay_us(3*HALF_BIT/4);
    return expected_second;
}

int16_t manchester_try_read() {
    // wait for sync
    uint8_t ones = 0;
    uint8_t bit;
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
    uint16_t ret = 0;
    for (uint8_t i = 0; i < 10; ++i) {
	bit = manchester_wait_bit();
	ret <<= 1;
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

uint16_t manchester_read() {
    int16_t t;
    while ((t = manchester_try_read()) < 0);
    return t;
}
