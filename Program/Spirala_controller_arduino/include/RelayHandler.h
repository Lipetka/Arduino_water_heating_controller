/**
 * @file RelayHandler.h
 * @author Samuel Liptak
 * @brief Header file for relay handling
 * @version 0.1
 * @date 2023-09-30
 * 
 */


#ifndef RELAY_HANDLER_H
#define RELAY_HANDLER_H

#include <Arduino.h>
#include <stdio.h>

extern bool heater_on_flag;
extern bool emergency_shutdown_flag;

void heater_relay_init();
void turn_heater_on();
void turn_heater_off();
void heater_error_handle();




#endif