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
#include "DisplayShow.h"
#include "EncoderHandler.h"
#include "TemperatureHandler.h"
#include "EEPROMHandler.h"

// DEFINES =============================================

#define DEBUG // keep defined

// Pins
#define RELAY_PIN 4


// RELAY control
#define RELAY_ON digitalWrite(RELAY_PIN,HIGH)
#define RELAY_OFF digitalWrite(RELAY_PIN,LOW)
#define MAX_RELAY_TIMER 21600000 // 6 hours
#define MIN_RELAY_TIMER 10800000 // 3 hours

// SCHEDULER CONTROL
// temperature reading period
#define TEMP_READ_PERIOD 1000 // [ms]


// FUNCTIONS ===========================================

// scheduler callback
void displayTaskCallback();
void readTempCallback();

// GLOBAL VARIABLES ====================================
unsigned long debugRefreshTimer = 0;

// Variables used by temperature monitoring
unsigned long temperature_read_rate = 10000;

// Variables used by display control
uint8_t changingValue = 0;
unsigned long displayRefreshTimer = 0; // used to control refresh rate of display
const int display_refresh_rate = 100;

// global control variables
int8_t currentPosition = 0;
uint8_t maxMenuCount = 2; // number of menu options (0 included, 2 => 3 items)

// OBJECTS =============================================
Scheduler scheduler;

Task displayRefreshTask(display_refresh_rate, TASK_FOREVER, &displayTaskCallback, &scheduler, true);
Task temperatureReadTask(temperature_read_rate, TASK_FOREVER, &readTempCallback, &scheduler, true);

// MAIN CODE ===============================================

void setup() {
  temperature_reading_init();
  // initialize pins
  //pinMode(ENCODER_BUTTON_PIN, INPUT);
  //pinMode(RELAY_PIN, OUTPUT);
  displayInit();  // initialize display (external file)
  Serial.begin(9600);
  scheduler.startNow();
}

void loop() {

  #ifdef DEBUG
  // debug USB output
  if(debugRefreshTimer + 1000 < millis()){
    // print time
    Serial.print("Current time: ");
    Serial.println("TBD");
    // output measured temperature
    Serial.print("Current measured temperature: ");
    Serial.println(get_current_temperature_info());
    // output other info about temperature
    Serial.print(" Lower limit: ");
    Serial.print(get_temperature_info(LOW_LIMIT));
    Serial.print("Higher limit: ");
    Serial.print(get_temperature_info(HIGH_LIMIT));
    Serial.print(" Temp offset: ");
    Serial.println(get_temperature_info(OFFSET));
    Serial.println("-----------------------");
    debugRefreshTimer = millis();
  }
  #endif
 
  // Handle encoder -------------------------------------------
  checkButtonPress();
  if(changingValue){
    changeValue(currentPosition);
  }else{
    menuSelect();
  }

  scheduler.execute();
}

void displayTaskCallback(){
  displayShow(currentPosition, changingValue);
}

void readTempCallback(){
  read_temperature();
}
