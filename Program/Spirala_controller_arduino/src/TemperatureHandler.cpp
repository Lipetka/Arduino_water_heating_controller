/**
 * @file TemperatureHandler.c
 * @author Samuel Liptak
 * @brief Source file for temperature handling
 * @version 0.1
 * @date 2023-09-30
 * 
 */


#include "TemperatureHandler.h"

// create objects

OneWire oneWire(3);
DallasTemperature sensors(&oneWire);

// set used pins

const int temp_reading_pin = 3;
const int fake_pullup_pin = 2;

void temperature_reading_init(){
  // define fake pullup pin as output and set it to +5V
  pinMode(fake_pullup_pin, OUTPUT);
  digitalWrite(fake_pullup_pin, HIGH);
}

void read_temperature(float* actual_temperature){
  // TODO: get temp in thread 
  sensors.requestTemperatures();
  *actual_temperature = sensors.getTempCByIndex(0) - actualTempOffset;
}

