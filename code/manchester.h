#ifndef MANCHESTER_H
#define MANCHESTER_H

#include <inttypes.h>

void manchester_init();
void manchester_send(uint16_t t);
uint16_t manchester_read();

// low level, for debug
int8_t manchester_wait_bit();
void manchester_send_bit(int8_t b);

#endif /* MANCHESTER_H */
