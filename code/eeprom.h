#ifndef EEPROM_H
#define EEPROM_H

#include <inttypes.h>

void eeprom_write (uint16_t where, uint8_t what);
uint8_t eeprom_read (uint16_t where);

#endif /* EEPROM_H */
