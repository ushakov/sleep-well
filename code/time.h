#ifndef TIME_H
#define TIME_H

#include <inttypes.h>

void time_init();
void time_check();
uint32_t time_get();
void time_set(uint32_t t); // in seconds since Monday

#endif /* TIME_H */
