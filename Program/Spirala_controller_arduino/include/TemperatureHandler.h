/**
 * @file TemperatureHandler.h
 * @author Samuel Liptak
 * @brief header file for temperature reading
 * @version 0.1
 * @date 2023-09-30
 * 
 */

#ifndef TEMPERATURE_HANDLER_H
#define TEMPERATURE_HANDLER_H

#include <Arduino.h>
#include <stdio.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROMHandler.h>

uint8_t lower_temperature_limit;
uint8_t high_temperature_limit;
uint8_t actual_temperature_offset;
float measured_temperature;

const uint8_t temp_reading_pin = 3;
const uint8_t fake_pullup_pin = 2;

void temperature_reading_init();
void read_temperature(float measured_temperature);

#endif
