/*
  Matrix Keypad Demo
  keypad-demo.ino
  Demonstrates use of 4x4 matrix membrane keypad with Arduino
  Results on serial monitor
 
  DroneBot Workshop 2020
  https://dronebotworkshop.com
*/
 
// Include the Keypad library
#include <Keypad.h>
 
// Constants for row and column sizes
const byte ROWS = 4;
const byte COLS = 3;
 
// Array to represent keys on keypad
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
 
// Connections to Arduino
byte rowPins[ROWS] = {A0, A1, A2, A3};
byte colPins[COLS] = {A4, A5, 13};
 
// Create keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
 
void setup() {
  // Setup serial monitor
  Serial.begin(9600);
}
 
void loop() {
  // Get key value if pressed
  char customKey = customKeypad.getKey();
 
  if (customKey) {
    // Print key value to serial monitor
    Serial.println(customKey);
  }
}
