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

// LOAD EXTERN VARIABLES
bool heater_on_flag = 0;
bool emergency_shutdown_flag = 0;

const int relay_pin = 4;
uint16_t relay_on_timer = 0;
uint16_t relay_off_timer = 0; // only used when emergency turned off heater
uint16_t max_on_time = 21600; // 6 hours in seconds
uint16_t min_off_time = 14400; // 4 hours in seconds

void heater_relay_init(){
    pinMode(relay_pin, OUTPUT);
    turn_heater_off();
}

void turn_heater_on(){
    if(emergency_shutdown_flag){
        turn_heater_off();
        return;
    }
    heater_on_flag = true;
    digitalWrite(relay_pin, HIGH);
}

void turn_heater_off(){
    heater_on_flag = false;
    digitalWrite(relay_pin, LOW);
}

void heater_error_handle(){
    // called every second
    if(emergency_shutdown_flag){
        relay_off_timer++;
        if(relay_off_timer >= min_off_time){
            emergency_shutdown_flag = 0; // reset flag to 0 and continue normal operation
            relay_on_timer = 0; // reset timer
        }
        return;
    }

    if(heater_on_flag){
        relay_on_timer++; // increases every second the heater is ON
        if(relay_on_timer >= max_on_time){
            emergency_shutdown_flag = 1;
        }
    }else{
        if(relay_on_timer == 0){
            return;
        }else{
            relay_on_timer--; // on and off timer will lineary increase and decrease
        }
    }
}