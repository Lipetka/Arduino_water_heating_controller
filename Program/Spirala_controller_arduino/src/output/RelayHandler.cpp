/**
 * @file RelayHandler.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "RelayHandler.h"

bool heater_on_flag;

void heater_relay_init(){
    pinMode(relay_pin, OUTPUT);
    turn_heater_off();
}

void turn_heater_on(){
    heater_on_flag = true;
    digitalWrite(relay_pin, HIGH);
}

void turn_heater_off(){
    heater_on_flag = false;
    digitalWrite(relay_pin, LOW);
}