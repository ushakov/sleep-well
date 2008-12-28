#ifndef ACCEL_H
#define ACCEL_H

#include <inttypes.h>

void accel_init();
// dumps 64 bytes
void accel_dump(uint8_t *buf);

// low-level
void accel_write(uint8_t where, uint8_t what);
uint8_t accel_read(uint8_t where);


#endif /* ACCEL_H */
