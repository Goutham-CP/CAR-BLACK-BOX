#ifndef EEPROM_H
#define EEPROM_H

/*
 * 24C02 EEPROM Slave Addresses
 */

#define EEPROM_WRITE    0xA0
#define EEPROM_READ     0xA1

/*
 * Function Prototypes
 */

void write_external_eeprom(unsigned char address,
                           unsigned char data);

unsigned char read_external_eeprom(unsigned char address);

#endif