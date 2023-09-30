/**
 * @file EncoderHandler.cpp
 * @author Samuel Liptak
 * @brief source file for encoder handling
 * @version 0.1
 * @date 2023-08-18
 * 
 */


#include "EncoderHandler.h"

Encoder myEncoder(ENCODER_A_PIN, ENCODER_B_PIN);

long oldEncoderValue = 0;

/**
 * @brief Handles menu item sweep with overflow protection.
 * 
 * 
 */ 
void menuSelect() {
    currentPosition += encoderValueRead();
    if(currentPosition < 0){
        currentPosition = 0;
    }else if(currentPosition > maxMenuCount){
        currentPosition = maxMenuCount;
    }
}


/**
 * @brief Uses encoder rotation to edit chosen menu value.
 * 
 * @param chosenPosition directly taken from chosen menu item to determine which
 * value to edit
 */
int changeValue(uint8_t chosenPosition){


  switch (chosenPosition)
  {
  case 0:
    lower_temperature_limit += encoderValueRead();
    return 1;

  case 1:
    actual_temperature_offset += encoderValueRead();
    return 1;
  
  case 2:
    high_temperature_limit += encoderValueRead();
    return 1;

  default:
    return 0;
  }

}

/**
 * @brief Checks if button has been pressed.
 * If yes, change the changingValue value and handle writing variables
 * to EEPROM when exiting edit mode.
 * 
 */
void checkButtonPress(){
    // check button press, ignore button press if press wasnt addressed yet -----------------
  if(digitalRead(ENCODER_BUTTON_PIN) == LOW && changingValue == 0){
    changingValue = 1;
    while(digitalRead(ENCODER_BUTTON_PIN) == LOW){}

  }else if(digitalRead(ENCODER_BUTTON_PIN) == LOW && changingValue == 1){
    changingValue = 0;
    myEncoder.write(0); // to reset arrow to zero position

    
    if(currentPosition == 0){
      set_lower_temperature_limit(lower_temperature_limit);
    }else if(currentPosition == 1){
      set_temperature_offset(actual_temperature_offset);
    }else if(currentPosition == 2){
      set_higher_temperature_limit(high_temperature_limit);
    }
    
    while(digitalRead(ENCODER_BUTTON_PIN) == LOW){} //TODO: get rid of while
  }
}

/**
 * @brief Returns increment or decrement of encoder with step of 1.
 * 
 * @return int 1,0 or -1
 */
int encoderValueRead(){

    long newEncoderValue = myEncoder.read(); // read current encoder position

    if(newEncoderValue > oldEncoderValue + 3){
        oldEncoderValue = newEncoderValue;
        return 1;
    }else if(newEncoderValue < oldEncoderValue - 3){
        oldEncoderValue = newEncoderValue;
        return -1;
    }else{
        return 0;
    }
}
