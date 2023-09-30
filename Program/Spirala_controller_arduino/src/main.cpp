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

// GLOBAL VARIABLES ====================================

unsigned long debugRefreshTimer = 0;

// EEPROM addresses (saved as variables to be accesses globally)
uint8_t TEMP_OFFSET_EEPROM_ADDRESS =  0xA0;
uint8_t TEMP_LOW_ADDRESS = 0xB0;
uint8_t TEMP_HIGH_ADDRESS = 0xC0;

// Variables used by temperature monitoring
unsigned long tempRefreshTimer = 1000;
uint16_t lowerTempLimit = EEPROM.get(TEMP_LOW_ADDRESS, lowerTempLimit); 
uint16_t higherTempLimit = EEPROM.get(TEMP_HIGH_ADDRESS, higherTempLimit);
int8_t actualTempOffset = EEPROM.get(TEMP_OFFSET_EEPROM_ADDRESS, actualTempOffset);
float tempActual = 0;

// Variables used by display control
uint8_t changingValue = 0;
unsigned long displayRefreshTimer = 0; // used to control refresh rate of display

// global control variables
int8_t currentPosition = 0;
uint8_t maxMenuCount = 2; // number of menu options (0 included, 2 => 3 items)

// OBJECTS =============================================

OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

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
}

void loop() {

  
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
 
  // Handle encoder -------------------------------------------
  checkButtonPress();
  if(changingValue){
    if(changeValue(currentPosition)){
      displayShow(lowerTempLimit, higherTempLimit, tempActual, currentPosition, changingValue);
    }
  }else{
    menuSelect();
  }

  // Update display every DISPLAY_REFRESH_RATE ms -----------------------------------
  if(displayRefreshTimer + DISPLAY_REFRESH_RATE < millis()){
    //TODO: display backlight autooff
    displayShow(lowerTempLimit, higherTempLimit, tempActual, currentPosition, changingValue);
    displayRefreshTimer = millis();
  }

  // Measure temperature every TEMP_READ_PERIOD ms -----------------------------------
  if(tempRefreshTimer + TEMP_READ_PERIOD < millis()){
    readTemp(&tempActual);
    tempRefreshTimer = millis(); // update timer
  }

  // Relay control ----------------------------------------------------------------
  // TODO: add continous ON state protection
  if(tempActual < lowerTempLimit){
    RELAY_ON;
  }else if(tempActual >= higherTempLimit){
    RELAY_OFF;
  }

  // millis Rollover handle --------------------------------------------------------
  // millis will rollover back to 0 after approx. 50 days. When this happens the code
  // managing display and temp reading timings will break becouse millis will always be 
  // smaller. To counter this, following code checks if refresh timers are smaller than millis, 
  // if yes the timers will catch up to millis. For this reason we dont have to update 
  if((tempRefreshTimer > millis())|| (displayRefreshTimer > millis()) || (debugRefreshTimer > millis())){
    tempRefreshTimer = millis();
    displayRefreshTimer = millis();
    debugRefreshTimer = millis();
    Serial.println("TIMERS HAVE RESETED");
  }

}


void readTemp(float *tempActual){
  // TODO: get temp in thread 
  sensors.requestTemperatures();
  *tempActual = sensors.getTempCByIndex(0) - actualTempOffset;
}

