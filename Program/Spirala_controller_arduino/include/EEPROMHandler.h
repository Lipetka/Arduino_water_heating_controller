

#ifndef EEPROM_HANDLER_H
#define EEPROM_HANDLER_H


#include <Arduino.h>
#include <stdio.h>
#include <EEPROM.h>

const uint8_t TEMP_OFFSET_EEPROM_ADDRESS = 0xA0;
const uint8_t TEMP_LOW_ADDRESS = 0xB0;
const uint8_t TEMP_HIGH_ADDRESS = 0xC0;

void read_temperature_constants(uint8_t* lower_temperature_limit, 
                            uint8_t* high_temperature_limit,
                            uint8_t* actual_temperature_offset);


void set_lower_temperature_limit(uint8_t* new_lower_temperature_limit);
void set_higher_temperature_limit(uint8_t* new_higher_temperature_limit);
void set_temperature_offset(uint8_t* new_offset);

#endif