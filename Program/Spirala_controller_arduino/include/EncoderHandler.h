/**
 * @file EncoderHandler.h
 * @author Samuel Liptak
 * @brief header file for encoder control
 * @version 0.1
 * @date 2023-08-18
 * 
 */

#ifndef ENCODER_HANDLER_H
#define ENCODER_HANDLER_H

#include <Arduino.h>
#include <stdio.h>
#include <Encoder.h>
#include <TemperatureHandler.h>
#include <EEPROMHandler.h>

const int ENCODER_BUTTON_PIN = 7;
const int ENCODER_A_PIN = 6;
const int ENCODER_B_PIN = 5;

extern uint8_t maxMenuCount;
extern int8_t currentPosition;
extern uint8_t changingValue;

void encoderInit();
void menuSelect();
void checkButtonPress();
int changeValue(uint8_t currentPosition);
int encoderValueRead();

#endif