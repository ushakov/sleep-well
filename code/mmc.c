/*! \file mmc.c \brief MultiMedia and SD Flash Card Interface. */
//*****************************************************************************
// Based on code from AVR-lib

#include <avr/io.h>			// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support
#include <inttypes.h>

#include "mmc.h"

#define SPI_PORT   PORTB
#define SPI_DDR    DDRB
#define SPI_PORTIN PINB
#define SPI_SCK    5
#define SPI_MISO   4
#define SPI_MOSI   3
#define SPI_SS     2
#define MMC_CS     0

#define SPI_START()   SPI_PORT &= ~(1 << MMC_CS)
#define SPI_END()     SPI_PORT |= (1 << MMC_CS)


// #define MMC_DEBUG 1

#ifdef MMC_DEBUG
#include <avr/pgmspace.h>
#include "put.h"

void debug_print(const char *s, uint16_t x) {
    putRam(s);
    putHexF(x, 2);
    putCRLF();
}

#define rprintf(str,x) debug_print(str,x)
#endif

static uint8_t spiTransferByte(uint8_t b)
{
    SPDR = b;
    int16_t count = 0;
    while(!(SPSR & (1 << SPIF))) {
	if (++count > 1024) {
	    #ifdef MMC_DEBUG
	    rprintf("can't send spi for # f cycles: 0x", count);
	    #endif
	    return 0xff;
	}
    }
    return SPDR;
}

void mmcInit() {
    SPI_DDR |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS) | (1 << MMC_CS);
    SPI_DDR &= ~(1 << SPI_MISO);
    
    /* Enable SPI, Master, set clock rate f/32 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPHA) | (1 << CPOL) | (1 << SPR1);
    SPSR = (1 << SPI2X);

    // Set idle SCK to high
    SPI_PORT |= (1 << SPI_SCK);
    SPI_PORT |= (1 << MMC_CS);
}

uint8_t mmcReset(void)
{
	uint8_t i;
	uint8_t retry;
	uint8_t r1=0;

	retry = 0;
	do
	{
		// send dummy bytes with CS high before accessing
		for(i=0;i<10;i++) spiTransferByte(0xFF);
		// resetting card, go to SPI mode
		r1 = mmcSendCommand(MMC_GO_IDLE_STATE, 0);
		#ifdef MMC_DEBUG
		rprintf("MMC_GO_IDLE_STATE: R1=0x", r1);
		#endif
		// do retry counter
		retry++;
		if(retry>10) return -1;
	} while(r1 != 0x01);

	// TODO: check card parameters for voltage compliance
	// before issuing initialize command

	retry = 0;
	do
	{
		// initializing card for operation
		r1 = mmcSendCommand(MMC_SEND_OP_COND, 0);
		#ifdef MMC_DEBUG
		rprintf("MMC_SEND_OP_COND: R1=0x", r1);
		#endif
		// do retry counter
		retry++;
		if(retry>100) return -1;
	} while(r1);
		
	// turn off CRC checking to simplify communication
	r1 = mmcSendCommand(MMC_CRC_ON_OFF, 0);
	#ifdef MMC_DEBUG
	rprintf("MMC_CRC_ON_OFF: R1=0x", r1);
	#endif

	// set block length to 512 bytes
	r1 = mmcSendCommand(MMC_SET_BLOCKLEN, 512);
	#ifdef MMC_DEBUG
	rprintf("MMC_SET_BLOCKLEN: R1=0x", r1);
	#endif

	// return success
	return 0;
}

uint8_t mmcSendCommand(uint8_t cmd, uint32_t arg)
{
	uint8_t r1;

	// assert chip select
	SPI_START();
	// issue the command
	r1 = mmcCommand(cmd, arg);
	// release chip select
	SPI_END();

	return r1;
}

uint8_t mmcRead(uint32_t sector, uint8_t* buffer)
{
	uint8_t r1;
	uint16_t i;

	// assert chip select
	SPI_START();
	// issue command
	r1 = mmcCommand(MMC_READ_SINGLE_BLOCK, sector<<9);
	#ifdef MMC_DEBUG
	rprintf("MMC Read Block R1=0x", r1);
	#endif
	// check for valid response
	if(r1 != 0x00) {
	    SPI_END();
	    return r1;
	}
	// wait for block start
	while(spiTransferByte(0xFF) != MMC_STARTBLOCK_READ);
	// read in data
	for(i=0; i<0x200; i++)
	{
		*buffer++ = spiTransferByte(0xFF);
	}
	// read 16-bit CRC
	spiTransferByte(0xFF);
	spiTransferByte(0xFF);
	// release chip select
	SPI_END();
	// return success
	return 0;
}

uint8_t mmcWrite(uint32_t sector, uint8_t* buffer)
{
	uint8_t r1;
	uint16_t i;

	// assert chip select
	SPI_START();
	// issue command
	r1 = mmcCommand(MMC_WRITE_BLOCK, sector<<9);
	#ifdef MMC_DEBUG
	rprintf("MMC Write Block R1=0x", r1);
	#endif
	// check for valid response
	if(r1 != 0x00) {
	    SPI_END();
	    return r1;
	}
	// send dummy
	spiTransferByte(0xFF);
	// send data start token
	spiTransferByte(MMC_STARTBLOCK_WRITE);
	// write data
	for(i=0; i<0x200; i++)
	{
		spiTransferByte(*buffer++);
	}
	// write 16-bit CRC (dummy values)
	spiTransferByte(0xFF);
	spiTransferByte(0xFF);
	// read data response token
	r1 = spiTransferByte(0xFF);
	if( (r1&MMC_DR_MASK) != MMC_DR_ACCEPT) {
	    SPI_END();
	    return r1;
	}
	#ifdef MMC_DEBUG
	rprintf("Data Response Token=0x", r1);
	#endif
	// wait until card not busy
	while(!spiTransferByte(0xFF));
	// release chip select
	SPI_END();
	// return success
	return 0;
}

uint8_t mmcCommand(uint8_t cmd, uint32_t arg)
{
	uint8_t r1;
	uint8_t retry=0;
	// send command
	spiTransferByte(cmd | 0x40);
	spiTransferByte(arg>>24);
	spiTransferByte(arg>>16);
	spiTransferByte(arg>>8);
	spiTransferByte(arg);
	spiTransferByte(0x95);	// crc valid only for MMC_GO_IDLE_STATE
	// end command
	// wait for response
	// if more than 8 retries, card has timed-out
	// return the received 0xFF
	while((r1 = spiTransferByte(0xFF)) == 0xFF)
		if(retry++ > 8) break;
	// return response
	return r1;
}
