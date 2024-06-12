// Copyright (C) 2024 Eshan Chathuranga.

// Licensed under the  GPL-3.0 License;
// you may not use this file except in compliance with the License.

// Thempeture Controller  - by Eshan Chathuranga

// Date - 2024/04/10
#include <LiquidCrystal_I2C.h>
#include <max6675.h>
#include <wire.h>
#include <EEPROM.h>

// Pin definitions
#define BUTTON_UP_PIN 7
#define BUTTON_DOWN_PIN 8
#define BUTTON_SET_PIN 9
#define RELAY_PIN 10
#define BUZER_PIN 11
#define POWER_LED_PIN 12

int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

// Constants
#define DEBOUNCE_DELAY 25 // milliseconds
#define TEMP_UPDATE_INTERVAL 500 // milliseconds

// LCD module settings
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2


// Variables
int targetTemp = 27; // Default target temperature in Celsius
unsigned long lastTempUpdate = 0;
bool relay1State = false;
bool relay2State = false;

// Objects
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
// MAX6675 thermocouple(4, 5, 6); // SO, CS, CLK pins


// Function prototypes void updateTemperature();
void updateLCD();

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  // Initialize buttons
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SET_PIN, INPUT_PULLUP);

  // Initialize relays
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZER_PIN, OUTPUT);
  pinMode(POWER_LED_PIN, OUTPUT);

  // Turn off relays initially
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(POWER_LED_PIN, HIGH);

  // StartUp
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp Controller");
  digitalWrite(BUZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZER_PIN, LOW);
  delay(500);
  digitalWrite(BUZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZER_PIN, LOW);
  delay(500);
  digitalWrite(BUZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZER_PIN, LOW);
  delay(500);
  digitalWrite(BUZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZER_PIN, LOW);
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("SKY7");
  delay(500);
  lcd.clear();
  delay(1000);

}

      void loop() {
             // Record the start time
             unsigned long startTime = millis();
             updateTempeture();
             buttonSetup();

 

         if (thermocouple.readCelsius() >  EEPROMReadInt(0) && !relay1State ) {
            switchOnRelay();
            relay1State = true;

          } else if (thermocouple.readCelsius() <=  EEPROMReadInt(0) && relay1State ) {
            switchOffRelay();
            relay1State = false;
          }

}





      void buttonSetup() {
        if (digitalRead(BUTTON_UP_PIN) == LOW) {
          delay(DEBOUNCE_DELAY);
              if (digitalRead(BUTTON_UP_PIN) == LOW) {
                  // Increase tempeture value
                  targetTemp++;
                  // Desplay Increase temperute value
                  lcd.setCursor(0, 1);
                  lcd.print("Target Temp:");
                  lcd.setCursor(13, 1);
                  lcd.print(targetTemp);
              }
          }
        if (digitalRead(BUTTON_DOWN_PIN) == LOW) {
          delay(DEBOUNCE_DELAY);
               if(digitalRead(BUTTON_DOWN_PIN) == LOW) {
                  // Decrease tempeture value
                  targetTemp--;
                  // Desplay Decrease temperute value
                  lcd.setCursor(0, 1);
                  lcd.print("Target Temp:");
                  lcd.setCursor(13, 1);
                  lcd.print(targetTemp);
               } 
          }
        if (digitalRead(BUTTON_SET_PIN) == LOW) {
          delay(DEBOUNCE_DELAY);
              if (digitalRead(BUTTON_SET_PIN) == LOW) {
                EEPROMWriteInt(0, targetTemp);
                lcd.setCursor(0, 1);
                lcd.print("Set Temp:");
                lcd.setCursor(11, 1);
                lcd.print("DONE!");
                delay(500);
              }
        }

      }

    void updateTempeture() {
        // thermocouple.readCelsius();
        delay(TEMP_UPDATE_INTERVAL);
        Serial.println(thermocouple.readCelsius());
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tempeture :");
        lcd.setCursor(12, 0);
        lcd.print(thermocouple.readCelsius());
      }

      void blinkLED() {
        digitalWrite(POWER_LED_PIN, HIGH);
        delay(1000);
        digitalWrite(POWER_LED_PIN, LOW);
        delay(1000);
      }
    
    // void updateLCD() {
    //       lcd.clear();
    //       lcd.setCursor(0, 0);
    //       lcd.print("Tempeture:");
    //       lcd.setCursor(10, 0);
    //       lcd.print();
    //  }

    void switchOnRelay() {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("UHC Section");
      digitalWrite(BUZER_PIN, HIGH);
      digitalWrite(POWER_LED_PIN, LOW);
      delay(1000);
      digitalWrite(BUZER_PIN, LOW);
      digitalWrite(POWER_LED_PIN, HIGH);
      delay(500);
      digitalWrite(BUZER_PIN, HIGH);
      digitalWrite(POWER_LED_PIN, LOW);
      delay(500);
      digitalWrite(BUZER_PIN, LOW);
      digitalWrite(POWER_LED_PIN, HIGH);
      delay(500);
      digitalWrite(BUZER_PIN, HIGH);
      digitalWrite(POWER_LED_PIN, LOW);
      delay(500);
      digitalWrite(BUZER_PIN, LOW);
      digitalWrite(POWER_LED_PIN, HIGH);
      delay(500);
      digitalWrite(BUZER_PIN, HIGH);
      delay(500);
      digitalWrite(BUZER_PIN, LOW);
      digitalWrite(RELAY_PIN, HIGH);
      lcd.setCursor(7, 1);
      lcd.print("ON");
      delay(2000);
    }

        void switchOffRelay() {
      digitalWrite(BUZER_PIN, HIGH);
      digitalWrite(POWER_LED_PIN, LOW);
      delay(1000);
      digitalWrite(BUZER_PIN, LOW);
      digitalWrite(POWER_LED_PIN, HIGH);
      digitalWrite(RELAY_PIN, LOW);

    }

   // Function to write an integer to EEPROM
    void EEPROMWriteInt(int address, int value) {
        EEPROM.put(address, value);
    }

  // Function to read an integer from EEPROM
  int EEPROMReadInt(int address) {
    int value = 0;
   EEPROM.get(address, value);
   return value;
  }     








































