/**
 * @file TemperatureHandler.h
 * @author Samuel Liptak
 * @brief header file for temperature reading
 * @version 0.1
 * @date 2023-09-30
 * 
 */

#ifndef TEMP_HANDLER_H
#define TEMP_HANDLER_H

#include <Arduino.h>
#include <stdio.h>
#include <OneWire.h>
#include <DallasTemperature.h>

void read_temperature(float* actual_temperature);
void temperature_reading_init();

#endif
