
#include "EEPROMHandler.h"

void read_temperature_constants(uint8_t* lower_temperature_limit, 
                            uint8_t* high_temperature_limit,
                            uint8_t* actual_temperature_offset){

    EEPROM.get(TEMP_OFFSET_EEPROM_ADDRESS, actual_temperature_offset);
    EEPROM.get(TEMP_LOW_ADDRESS, lower_temperature_limit);
    EEPROM.get(TEMP_HIGH_ADDRESS, high_temperature_limit);                            
}


void set_lower_temperature_limit(uint8_t* new_lower_temperature_limit){
    EEPROM.put(TEMP_LOW_ADDRESS, *new_lower_temperature_limit);
}

void set_higher_temperature_limit(uint8_t* new_higher_temperature_limit){
    EEPROM.put(TEMP_HIGH_ADDRESS, *new_higher_temperature_limit);
}

void set_temperature_offset(uint8_t* new_offset){
    EEPROM.put(TEMP_OFFSET_EEPROM_ADDRESS, *new_offset);
}