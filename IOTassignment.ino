
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

// Stores whether the buzzer is currently sounding.
bool buzzerOn = false;

void setup() {                    // Runs once when the Arduino starts.

  pinMode(ledPin, OUTPUT);        // Sets the LED pin as an output.
  pinMode(buzzerPin, OUTPUT);     // Sets the buzzer pin as an output.
  pinMode(buttonPin, INPUT);      // Sets the button pin as an input.

  Serial.begin(9600);             // Starts the Serial Monitor at 9600 baud.

  lcd.begin(16, 2);               // Starts the 16-column, 2-row LCD.
  lcd.setRGB(0, 255, 0);          // Sets the LCD backlight to green.
  lcd.setCursor(0, 0);            // Selects the first row.
  lcd.print("PARKING ASSIST");    // Displays the project title.
  lcd.setCursor(0, 1);            // Selects the second row.
  lcd.print("Starting...");       // Displays a startup message.
  delay(1500);                    // Keeps the startup message visible briefly.
  lcd.clear();                    // Clears the startup message.
}

void loop() {                     // Repeats continuously while powered.

  // Reads the current state of the Grove button.
  bool buttonState = digitalRead(buttonPin);

  // Checks whether the button has just been pressed.
  if (lastButtonState == LOW &&
      buttonState == HIGH &&
      millis() - lastPress > 250) {

    systemOn = !systemOn;         // Toggles the system on or off.
    lastPress = millis();         // Records the button press time.
  }
