/**
 * @file TemperatureHandler.c
 * @author Samuel Liptak
 * @brief Source file for temperature handling
 * @version 0.2
 * @date 2023-09-30
 * 
 */


#include <TemperatureHandler.h>

uint8_t lower_temperature_limit;
uint8_t high_temperature_limit;
int8_t actual_temperature_offset;
float measured_temperature;

// create objects
OneWire oneWire(temp_reading_pin);
DallasTemperature sensors(&oneWire);



void temperature_reading_init(){
  // define fake pullup pin as output and set it to +5V
  pinMode(fake_pullup_pin, OUTPUT);
  digitalWrite(fake_pullup_pin, HIGH);
  lower_temperature_limit = get_temperature_from_EEPROM(LOW_LIMIT);
  high_temperature_limit = get_temperature_from_EEPROM(HIGH_LIMIT);
  actual_temperature_offset = get_temperature_from_EEPROM(OFFSET);
}


void read_temperature(){
  sensors.requestTemperatures();
  measured_temperature = sensors.getTempCByIndex(0);
}


int get_temperature_info(uint8_t requested_value){
  switch (requested_value)
  {
  case LOW_LIMIT:
    return lower_temperature_limit;
    break;
  case HIGH_LIMIT:
    return high_temperature_limit;
    break;
  case OFFSET:
    return actual_temperature_offset;
    break;
  
  default:
    break;
  }
}


float get_calibrated_temperature_info(){
  return measured_temperature + actual_temperature_offset;
}
