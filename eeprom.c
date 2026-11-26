#include<xc.h>
#include"black_box.h"
#include"ds1307.h"
#include"clcd.h"
#include"i2c.h"
#include"adc.h"
#include"matrix_keypad.h"
#include"eeprom.h"
#include<string.h>
#include"uart.h"

void write_external_eeprom(unsigned char address,
                           unsigned char data)
{
    i2c_start();

    i2c_write(EEPROM_WRITE);

    i2c_write(address);

    i2c_write(data);

    i2c_stop();

    __delay_ms(5);
}

unsigned char read_external_eeprom(unsigned char address)
{
    unsigned char data;

    i2c_start();

    i2c_write(EEPROM_WRITE);

    i2c_write(address);

    i2c_rep_start();

    i2c_write(EEPROM_READ);

    data = i2c_read();

    i2c_stop();

    return data;
}