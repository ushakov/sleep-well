#include <avr/io.h>

#ifdef SENSOR

#define DDR_LED DDRA
#define PORT_LED PORTA
#define BIT_LED PORTA0

#define DDR_TR DDRA
#define PORT_TR PORTA
#define BIT_TR PORTA1

#define DDR_RADIO DDRA
#define PORT_RADIO PORTA
#define PIN_RADIO PINA
#define BIT_RADIO PORTA7

#else   // SENSOR

#define DDR_RADIO DDRB
#define PORT_RADIO PORTB
#define PIN_RADIO PINB
#define BIT_RADIO PORTB3

#endif  // SENSOR
