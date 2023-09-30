

#ifndef EEPROM_HANDLER_H
#define EEPROM_HANDLER_H


#include <Arduino.h>
#include <stdio.h>
#include <EEPROM.h>

#define LOW_LIMIT 0
#define HIGH_LIMIT 1
#define OFFSET 2

const uint8_t TEMP_OFFSET_EEPROM_ADDRESS = 0xA0;
const uint8_t TEMP_LOW_ADDRESS = 0xB0;
const uint8_t TEMP_HIGH_ADDRESS = 0xC0;

void set_lower_temperature_limit(uint8_t new_lower_temperature_limit);
void set_higher_temperature_limit(uint8_t new_higher_temperature_limit);
void set_temperature_offset(int8_t new_offset);
int get_temperature_from_EEPROM(uint8_t requested_data);

#endif