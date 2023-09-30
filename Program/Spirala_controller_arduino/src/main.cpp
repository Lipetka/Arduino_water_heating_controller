/**
 * @file main.cpp
 * @author Samuel Liptak
 * @brief Main file for hysteresis (bang-bang, 2 step,...) controller
 * @version 0.1
 * @date 2023-09-30
 * 
 */

// INCLUDES ============================================

// BASIC LIBRARIES
#include <Arduino.h>
#include <Wire.h> 
#include <stdio.h>
#include "TaskScheduler.h"

// PROJECT SPECIFIC LIBRARIES
#include "DisplayHandler.h"
#include "EncoderHandler.h"
#include "TemperatureHandler.h"
#include "EEPROMHandler.h"
#include "RelayHandler.h"

// DEFINES =============================================

#define DEBUG // keep defined

#define MAX_RELAY_TIMER 21600000 // 6 hours
#define MIN_RELAY_TIMER 10800000 // 3 hours

// FUNCTIONS ===========================================

// scheduler callback
void displayTaskCallback();
void readTempCallback();
void encoderTaskCallback();
void debugTaskCallback();

// GLOBAL VARIABLES ====================================
unsigned long debugRefreshTimer = 0;

// Variables used by display control
uint8_t changingValue = 0;

const int display_refresh_rate = 250;
const unsigned long temperature_read_rate = 10000;
const uint8_t encoder_read_rate = 10;
const uint16_t debug_print_rate = 1000;

// global control variables
int8_t currentPosition = 0;
uint8_t maxMenuCount = 2; // number of menu options (0 included, 2 => 3 items)

// OBJECTS =============================================
Scheduler scheduler;

Task displayRefreshTask(display_refresh_rate, TASK_FOREVER, &displayTaskCallback, &scheduler, true);
Task temperatureReadTask(temperature_read_rate, TASK_FOREVER, &readTempCallback, &scheduler, true);
Task encoderTask(encoder_read_rate, TASK_FOREVER, &encoderTaskCallback, &scheduler, true);
Task debugTask(debug_print_rate, TASK_FOREVER, &debugTaskCallback, &scheduler, true);

// MAIN CODE ===============================================

void setup() {
  // Initiate all modules
  temperature_reading_init();
  heater_relay_init();
  encoderInit();
  displayInit();  
  // begin serial output
  Serial.begin(9600);
  // setup scheduler
  scheduler.startNow();
}

void loop() {

  scheduler.execute();

}

void encoderTaskCallback(){
  checkButtonPress();
  if(changingValue){
    changeValue(currentPosition);
  }else{
    menuSelect();
  }
}

void displayTaskCallback(){
  displayShow(currentPosition, changingValue);
}

void readTempCallback(){
  // read temperature
  read_temperature();

  // turn heater on or off
  if(get_calibrated_temperature_info() < lower_temperature_limit){
    turn_heater_on();
  }else if(get_calibrated_temperature_info() > high_temperature_limit){
    turn_heater_off();
  }

}

void debugTaskCallback(){
    // print time
    Serial.print("Current time: ");
    Serial.println("TBD");
    // output measured temperature
    Serial.print("Current measured temperature: ");
    Serial.println(measured_temperature);
    // output other info about temperature
    Serial.print("Lower limit: ");
    Serial.print(get_temperature_info(LOW_LIMIT));
    Serial.print(" Higher limit: ");
    Serial.print(get_temperature_info(HIGH_LIMIT));
    Serial.print(" Temp offset: ");
    Serial.println(get_temperature_info(OFFSET));
    Serial.print("Relay state: ");
    Serial.println(heater_on_flag);
    Serial.println("-----------------------");
    debugRefreshTimer = millis();
}
