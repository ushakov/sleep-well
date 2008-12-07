#include <util/delay.h>

#include "delay.h"

void delay_ms (int ms) {
    uint32_t cycles_to_do = (uint32_t)  ((F_CPU) / 4000 * ms);
    while (cycles_to_do > 60000) {
	_delay_loop_2 (60000);
	cycles_to_do -= 60000;
    }
    _delay_loop_2 ((uint16_t) cycles_to_do);
}

void delay_s (int s) {
    for (int i = 0; i < s; ++i) {
	delay_ms (1000);
    }
}

#if F_CPU != 8000000UL
#error delay_us is tuned to 8MHz!
#endif
void delay_us(uint16_t us)
{
    int16_t loops = 8 * us / 3;
    while (loops > 256) {
	_delay_loop_1 (0);
	loops -= 256;
    }
    _delay_loop_1 ((uint8_t) loops);
}

