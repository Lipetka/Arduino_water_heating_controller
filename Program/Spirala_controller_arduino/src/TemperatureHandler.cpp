/**
 * @file TemperatureHandler.c
 * @author Samuel Liptak
 * @brief Source file for temperature handling
 * @version 0.1
 * @date 2023-09-30
 * 
 */


#include <TemperatureHandler.h>

// create objects

OneWire oneWire(temp_reading_pin);
DallasTemperature sensors(&oneWire);

void temperature_reading_init(){
  // define fake pullup pin as output and set it to +5V
  pinMode(fake_pullup_pin, OUTPUT);
  digitalWrite(fake_pullup_pin, HIGH);
  read_temperature_constants(&lower_temperature_limit, &high_temperature_limit, &actual_temperature_offset);
}

void read_temperature(float measured_temperature){
  sensors.requestTemperatures();
  measured_temperature = sensors.getTempCByIndex(0) - actual_temperature_offset;
}

