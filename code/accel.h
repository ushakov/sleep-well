#ifndef ACCEL_H
#define ACCEL_H

#include <inttypes.h>

void accel_init();

// low-level
void accel_write(uint8_t where, uint8_t what);
uint8_t accel_read(uint8_t where);

#ifdef ACCEL_DUMP
// dumps 64 bytes
void accel_dump(uint8_t *buf);
#endif

//high level
void accel_set_param();
int16_t accel_get_overall();

#endif /* ACCEL_H */
