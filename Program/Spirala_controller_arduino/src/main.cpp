/**
 * @file main.cpp
 * @author Samuel Liptak
 * @brief Main file for hysteresis (bang-bang, 2 step,...) controller
 * @version 0.1
 * @date 2023-09-30
 * 
 */
// INCLUDES ============================================

#include <Arduino.h>
#include <Wire.h> 
#include <stdio.h>
#include "DisplayShow.h"
#include "EncoderHandler.h"
#include <stdio.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "TaskScheduler.h"


// DEFINES =============================================

#define DEBUG

// Pins
#define RELAY_PIN 4
#define TEMP_PIN 3
#define FAKE_PULLUP 2 // used for setting pullup resistor

// temperature reading period
#define TEMP_READ_PERIOD 1000 // [ms]

// RELAY control
#define RELAY_ON digitalWrite(RELAY_PIN,HIGH)
#define RELAY_OFF digitalWrite(RELAY_PIN,LOW)
#define MAX_RELAY_TIMER 21600000 // 6 hours
#define MIN_RELAY_TIMER 10800000 // 3 hours



// FUNCTIONS ===========================================

void readTemp(float* actualTemperature);

// scheduler callback
void displayTaskCallback();
void readTempCallback();

// GLOBAL VARIABLES ====================================

unsigned long debugRefreshTimer = 0;

// EEPROM addresses (saved as variables to be accesses globally)
uint8_t TEMP_OFFSET_EEPROM_ADDRESS =  0xA0;
uint8_t TEMP_LOW_ADDRESS = 0xB0;
uint8_t TEMP_HIGH_ADDRESS = 0xC0;

// Variables used by temperature monitoring
unsigned long temperature_read_rate = 10000;
uint16_t lowerTempLimit = EEPROM.get(TEMP_LOW_ADDRESS, lowerTempLimit); 
uint16_t higherTempLimit = EEPROM.get(TEMP_HIGH_ADDRESS, higherTempLimit);
int8_t actualTempOffset = EEPROM.get(TEMP_OFFSET_EEPROM_ADDRESS, actualTempOffset);
float tempActual = 0;

// Variables used by display control
uint8_t changingValue = 0;
unsigned long displayRefreshTimer = 0; // used to control refresh rate of display
const int display_refresh_rate = 100;

// global control variables
int8_t currentPosition = 0;
uint8_t maxMenuCount = 2; // number of menu options (0 included, 2 => 3 items)

// OBJECTS =============================================

OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

Scheduler scheduler;

Task displayRefreshTask(display_refresh_rate, TASK_FOREVER, &displayTaskCallback, &scheduler, true);
Task temperatureReadTask(temperature_read_rate, TASK_FOREVER, &readTempCallback, &scheduler, true);

// MAIN CODE ===============================================

void setup() {
  // initialize pins
  pinMode(ENCODER_BUTTON_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(FAKE_PULLUP, OUTPUT);
  digitalWrite(FAKE_PULLUP, HIGH);
  sensors.begin();
  displayInit();  // initialize display (external file)
  readTemp(&tempActual); // read temp to start system with correct temperature
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
    Serial.println(tempActual);
    // output other info about temperature
    Serial.print("Higher limit: ");
    Serial.print(higherTempLimit);
    Serial.print(" Lower limit: ");
    Serial.print(lowerTempLimit);
    Serial.print(" Temp offset: ");
    Serial.println(actualTempOffset);
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

  // Relay control ----------------------------------------------------------------
  // TODO: add continous ON state protection
  if(tempActual < lowerTempLimit){
    RELAY_ON;
  }else if(tempActual >= higherTempLimit){
    RELAY_OFF;
  }
}


void readTemp(float *tempActual){
  // TODO: get temp in thread 
  sensors.requestTemperatures();
  *tempActual = sensors.getTempCByIndex(0) - actualTempOffset;
}

void displayTaskCallback(){
  displayShow(lowerTempLimit, higherTempLimit, tempActual, currentPosition, changingValue);
}

void readTempCallback(){
  readTemp(&tempActual);
}