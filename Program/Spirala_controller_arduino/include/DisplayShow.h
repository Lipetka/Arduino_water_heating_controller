/**
 * @file DisplayShow.h
 * @author Samuel Liptak
 * @brief header file for display control
 * @version 0.1
 * @date 2023-08-18
 * 
 */

#ifndef DISPLAY_SHOW_H
#define DISPLAY_SHOW_H

#include <Arduino.h>
#include <stdio.h>
#include <LiquidCrystal_I2C.h>

#define DISPLAY_REFRESH_RATE 300

void displayShow(uint16_t tempLimitLow, uint16_t tempLimitHigh, float tempActual, uint8_t chosenVariable, uint8_t changingValue);
void displayInit();


#endif