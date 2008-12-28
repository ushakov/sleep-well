#include <inttypes.h>
#include <avr/io.h>
#include "delay.h"
#include "accel.h"

#define SPI_PORT   PORTB
#define SPI_DDR    DDRB
#define SPI_PORTIN PINB
#define SPI_SCK    5
#define SPI_MISO   4
#define SPI_MOSI   3
#define SPI_SS     2

static uint8_t accel_transmit(uint8_t b);

#define SPI_START   SPI_PORT &= ~(1 << SPI_SS)
#define SPI_END     SPI_PORT |= (1 << SPI_SS)

void accel_init() {
    SPI_DDR |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);
    SPI_DDR &= ~(1 << SPI_MISO);
    
    /* Enable SPI, Master, set clock rate f/2 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPHA) | (1 << CPOL);
    // f/128
//    SPCR |= (1 << SPR1) | (1 << SPR0);
//    SPSR = (1 << SPI2X);

    // Set idle SCK to high
    SPI_PORT |= (1 << SPI_SCK);
    SPI_PORT |= (1 << SPI_SS);
}

static uint8_t accel_transmit(uint8_t b)
{
    SPDR = b;
    while(!(SPSR & (1 << SPIF)));
    return SPDR;
}

void accel_write(uint8_t where, uint8_t what) {
    SPI_START;
    accel_transmit(where);
    accel_transmit(what);
    SPI_END;
}

uint8_t accel_read(uint8_t where) {
    SPI_START;
    accel_transmit(0x80 + where);
    uint8_t ret = accel_transmit(0);
    SPI_END;
    return ret;
}


// dumps 64 bytes
void accel_dump(uint8_t *buf) {
    SPI_START;
    accel_transmit(0xc0);   // read, increment, addr 0/
//    accel_transmit(0x80 + 0x27);   // read, no-increment, addr 0x27/
//    while(1) {
//	accel_transmit(0x55);
//    }
    for (int i = 0; i < 64; ++i) {
	buf[i] = accel_transmit(0);
    }
    SPI_END;
}
