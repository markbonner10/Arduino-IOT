
#include <Wire.h>                 // Enables I2C communication with the LCD.
#include "rgb_lcd.h"              // Loads the Grove RGB LCD V5.0 library.

rgb_lcd lcd;                      // Creates the LCD display object.

// Assigns the Arduino pins to the Grove modules.
const int lightPin = A0;          // Reads the Grove light sensor.
const int ledPin = 3;             // Controls the Grove LED module.
const int buzzerPin = 4;          // Controls the Grove buzzer module.
const int buttonPin = 2;          // Reads the Grove button module.

bool systemOn = true;             // Stores whether the parking warning system is switched on.
bool lastButtonState =  LOW;      // Stores the previous button state for detecting a new press.
unsigned long lastPress = 0;      // Records when the button was last pressed.
unsigned long lastBeep = 0;       // Records when the buzzer last changed state

bool buzzerOn = false;            // Stores whether the buzzer is currently sounding
void setup() {                    // Runs once the Arduino starts.

pinMode(ledpin, OUTPUT);          // Sets the LED pin as an output.
pinMode(buzzerPin, OUTPUT);       // Sets the buzzer pin as an output
pinMode(buttonPin, INPUT);        // Sets the button pin as an input

