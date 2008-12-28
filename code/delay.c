#include <util/delay.h>

#include "delay.h"

#if F_CPU != 8000000UL
#error delay_us is tuned to 8MHz!
#endif

void delay_ms (int16_t ms) {
    for(int16_t i = 0; i < ms; ++i) {
	delay_us(1000);
    }
}

void delay_s (int16_t s) {
    for (int i = 0; i < s; ++i) {
	delay_ms (1000);
    }
}

void delay_us(uint16_t us)
{
    int16_t loops = 8 * us / 3;
    while (loops > 256) {
	_delay_loop_1 (0);
	loops -= 256;
    }
    _delay_loop_1 ((uint8_t) loops);
}

