/**
 * @file DisplayShow.cpp
 * @author Samuel Liptak
 * @brief display controller cpp file
 * @version 0.1
 * @date 2023-08-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "DisplayShow.h"

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

/**
 * @brief Initiates LCD display
 * 
 */
void displayInit(){
    lcd.init();                      // initialize the lcd 
    lcd.backlight();                 // turn on backlight
}

/**
 * @brief Controls what will be shown on display
 * 
 * @param tempLimitLow Lower temp limit
 * @param tempLimitHigh Higher temp limit
 * @param tempActual Current measured temperature
 * @param chosenVariable Current menu position
 * @param changingValue Is chosen variable being changed?
 */
void displayShow(uint16_t tempLimitLow, uint16_t tempLimitHigh, float tempActual, uint8_t chosenVariable, uint8_t changingValue){
    lcd.clear();

    lcd.setCursor(0,0); // COL, ROW
    lcd.print("MIN");
    lcd.setCursor(0,1);
    lcd.print(tempLimitLow);

    lcd.setCursor(5,0);
    lcd.print("AKT");
    lcd.setCursor(5,1);
    lcd.print(tempActual,2);


    lcd.setCursor(12,0);
    lcd.print("MAX");
    lcd.setCursor(12,1);
    lcd.print(tempLimitHigh);

    // highlight handle
    switch (chosenVariable)
    {
    case 0:

        lcd.setCursor(2,1);
        if(changingValue){
            lcd.blink_on();
        }else{
            lcd.blink_off();
        }
        lcd.print("<-");
        lcd.setCursor(2,1);
        break;

    case 1:
        lcd.setCursor(7,1);
        if(changingValue){
            lcd.blink_on();
        }else{
            lcd.blink_off();
        }
        lcd.print("<-");
        lcd.setCursor(7,1);
        break;

    case 2:
        lcd.setCursor(14,1);
        if(changingValue){
            lcd.blink_on();
        }else{
            lcd.blink_off();
        }
        lcd.print("<-");
        lcd.setCursor(14,1);
        break;

    default:
        break;
    }

}