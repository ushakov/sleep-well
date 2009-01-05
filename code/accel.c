#include <inttypes.h>
#include <avr/io.h>
#include "delay.h"
#include "accel.h"

#include "put.h"

#define SPI_PORT   PORTB
#define SPI_DDR    DDRB
#define SPI_PORTIN PINB
#define SPI_SCK    5
#define SPI_MISO   4
#define SPI_MOSI   3
#define SPI_SS     2

#define ACC_PORT   PORTC
#define ACC_DDR    DDRC
#define ACC_CS     1

static uint8_t accel_transmit(uint8_t b);

#define ACC_START   ACC_PORT &= ~(1 << ACC_CS)
#define ACC_END     ACC_PORT |= (1 << ACC_CS)

void accel_init() {
    // Assumes SPI is already initialized!
    // If you eed to initialize it here, uncomment these:
//    SPI_DDR |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);
//    SPI_DDR &= ~(1 << SPI_MISO);
//    // Enable SPI, Master, set clock rate f/2
//    SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPHA) | (1 << CPOL);
//    // Set idle SCK to high
//    SPI_PORT |= (1 << SPI_SCK);

    ACC_DDR |= (1 << ACC_CS);
    ACC_END;
}

static uint8_t accel_transmit(uint8_t b)
{
    SPDR = b;
    int16_t count = 0;
    while(!(SPSR & (1 << SPIF))) {
	if (++count > 1024) {
	    return 0xff;
	}
    }
    return SPDR;
}

void accel_write(uint8_t where, uint8_t what) {
    ACC_START;
    accel_transmit(where);
    accel_transmit(what);
    ACC_END;
}

uint8_t accel_read(uint8_t where) {
    ACC_START;
    accel_transmit(0x80 + where);
    uint8_t ret = accel_transmit(0);
    ACC_END;
    return ret;
}

#ifdef ACCEL_DUMP
// dumps 64 bytes
void accel_dump(uint8_t *buf) {
    ACC_START;
    accel_transmit(0xc0);   // read, increment, addr 0/
//    accel_transmit(0x80 + 0x27);   // read, no-increment, addr 0x27/
//    while(1) {
//	accel_transmit(0x55);
//    }
    for (int i = 0; i < 64; ++i) {
	buf[i] = accel_transmit(0);
    }
    ACC_END;
}
#endif

static int16_t get_accel(int8_t base_reg) {
    int16_t r = accel_read(base_reg);
    r |= (accel_read(base_reg+1) << 8);
    return r;
}

static int16_t my_sqrt(int32_t n) {
    int32_t op = n;
    int32_t res = 0;
    int32_t one = (1L << 30);
 
    // putProg("b0: n=");
    // putHexF(n,2);
    // putProg(" one=");
    // putHexF(one,2);
    // putCRLF();

    // "one" starts at the highest power of four <= the argument.
    while (one > op) {
	one >>= 2;
    }
    // putProg("b1:");
    // putHexF(one,2);
    // putCRLF();
    
    while (one != 0) {
	// putProg("b2: op=");
	// putHexF(op,2);
	// putProg(" one=");
	// putHexF(one,2);
	// putProg(" res=");
	// putHexF(res,2);
	// putCRLF();
	if (op >= res + one) {
	    op -= res + one;
	    res += one << 1;
	}
	res >>= 1;
	one >>= 2;
    }
    return res;
}

// high level

void accel_set_param() {
    accel_write(0x20, 0xc7);
}

int16_t accel_get_overall() {
    int32_t x, y, z;
    int16_t count = 0;
    int8_t bigcount = 0;
    // putProg("a1\r\n");
    while((accel_read(0x27) & 0x8) == 0) {
	count++;
	if (count > 1000) {
	    // putProg("a2\r\n");
	    accel_set_param();
	    if (accel_read(0x0f) != 0x3a) {
		return -2;
	    }
	    bigcount++;
	    count = 0;
	}
	if (bigcount > 10) {
	    // putProg("a3\r\n");
	    return -1;
	}
    }
    // putProg("a4:");
    // putHexF(count,2);
    // putCRLF();

    x = get_accel(0x28);
    // putProg("a5:");
    // putHexF(x,2);
    // putCRLF();
    y = get_accel(0x2a);
    // putProg("a6:");
    // putHexF(y,2);
    // putCRLF();
    z = get_accel(0x2c);
    // putProg("a7:");
    // putHexF(z,2);
    // putCRLF();
    int32_t sq = x*x + y*y + z*z;
    // putProg("a8:");
    // putHexF(sq,2);
    // putCRLF();
    int16_t ret = my_sqrt(sq);
    // putProg("a9:");
    // putHexF(ret,2);
    // putCRLF();
    return ret;
}
