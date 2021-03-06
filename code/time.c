#include <avr/io.h>
#include <avr/interrupt.h>
#include "time.h"

uint32_t time;    // in seconds since Monday
uint16_t last_check; // last checked timer value (for overflow checks)

ISR(TIMER1_COMPA_vect) {
    time++;
    last_check = 0;
    sei();
}

void time_init() {
    uint16_t top = F_CPU / 256;
    TCCR1B = 0;   // stop timer
    OCR1A = top;
    TCNT1 = 0;
    // start timer, set mode
    TCCR1B = (1 << WGM12) | (1 << CS12);
    TCCR1A = 0;
    // enable interrupts
    TIMSK |= (1 << OCIE1A);
    last_check = 0;
}

void time_check() {
    uint16_t t = TCNT1;
    if (t < last_check) {
	// overflow occured (at least once)
	time++;
    }
    last_check = t;
}

// returns seconds since monday
uint32_t time_get() {
    return time % 604800UL;
}

void time_set(uint32_t t) {
    time = t;
}
