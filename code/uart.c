#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"

static int errors;

#define BUFLEN 16
#define BUFLENMASK 15
static uint8_t buffer[BUFLEN];
static uint8_t volatile bstart;
static uint8_t volatile blen;

ISR(USART_RXC_vect) {
    uint8_t data = UDR;
    uint8_t place = (bstart + blen) & BUFLENMASK;
    buffer[place] = data;
    blen++;
    sei();
}

void uart_init() {
    DDRD &= ~(1 << PIND0);
    DDRD |= (1 << PIND1);
    UCSRA = 0; //(1 << U2X);
    UCSRB = (1 << RXEN ) | (1 << TXEN) | (1 << RXCIE);
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0); // | (1 << UPM1) | (1 << UPM0) 
    UBRRH = 0;
    UBRRL = 25;
    bstart = 0;
    blen = 0;
    errors = 0;
    PIND |= (1 << PIND0);
    sei();
}

void uart_off() {
    UCSRA = 0;
    UCSRB = 0;
    UCSRC = 0;
    sei();
}

// set the uart baud rate
void uart_set_baud_rate(uint32_t baudrate)
{
    // calculate division factor for requested baud rate, and set it
    uint16_t bauddiv = (F_CPU)/(baudrate*16L)-1;
    UBRRL = bauddiv;
#ifdef UBRRH
    UBRRH = bauddiv>>8;
#endif
}

void uart_write_byte (uint8_t data) {
    DDRA |= (1 << PORTA0);
    while ( !(UCSRA & (1 << UDRE)) ) {
	PORTA |= (1 << PORTA0);
	PORTA &= ~(1 << PORTA0);
    }
    UDR = data;
}

int16_t uart_getchar()
{
    if (blen > 0) {
	char c = buffer[bstart];
	cli();
	bstart = (bstart + 1) & BUFLENMASK;
	blen--;
	sei();
	return c;
    }
    return -1;
}

