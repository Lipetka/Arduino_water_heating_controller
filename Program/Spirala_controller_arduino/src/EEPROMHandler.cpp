
#include "EEPROMHandler.h"

int get_temperature_from_EEPROM(uint8_t requested_data){
    uint8_t return_value;

    switch (requested_data)
    {
    case LOW_LIMIT:
        EEPROM.get(TEMP_LOW_ADDRESS, return_value);
        break;

    case HIGH_LIMIT:
        EEPROM.get(TEMP_HIGH_ADDRESS, return_value);
        break; 

    case OFFSET:
        EEPROM.get(TEMP_OFFSET_EEPROM_ADDRESS, return_value);
        break;

    default:
        return_value = 0;
        break;
    }

    return return_value;                    
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