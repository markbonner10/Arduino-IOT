
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

  lastButtonState = buttonState;  // Saves the button state for the next loop.

  // Handles the system when it has been switched off.
  if (!systemOn) {

    digitalWrite(ledPin, LOW);    // Turns off the LED.
    digitalWrite(buzzerPin, LOW); // Turns off the buzzer.
    buzzerOn = false;             // Records that the buzzer is off.

    lcd.setRGB(0, 0, 80);         // Sets the backlight to dim blue.
    lcd.setCursor(0, 0);          // Selects the first row.
    lcd.print("PARKING ASSIST "); // Displays the project title.
    lcd.setCursor(0, 1);          // Selects the second row.
    lcd.print("SYSTEM OFF      ");// Displays the system status.

    delay(50);                    // Waits briefly before checking again.
    return;                       // Skips the remaining code while switched off.
  }

  // Reads the light sensor; readings normally range from 0 to 1023.
  int light = analogRead(lightPin);

  // Sets the default buzzer interval to zero, meaning no warning.
  int beepInterval = 0;

// Creates a variable for the message shown on the LCD const chat* status;

// Checks whether the sensor is receiving plenty of light. if (light > 700) {
status = "SAFE";                   // Displays SAFE
beepInterval = 700;                // Keeps the buzzer silent.
lcd.setRGB(255, 180, 0);           // Sets the LED backlight to green

// Checks whether the sensor is reading moderately low.
} else if (light > 450) {
Status = "CAUTION"                 // Displays caution.
beepInterval = 700;                // Sets a slow beep rate.
lcd.setRGB(255, 180, 0);           // Sets the LED backlight to orange

// Checks whether the sensor reading is lower still.
} else if (light > 250) {
status = "WARNING";                // Displays WARNING.
beepInterval = 350;                // Sets a Faster beep rate.
lcd.setRGB(255,60,0);              // Sets the LCD backlight to on

//Runs when the sensor reading is 250 or below.
} else {
status = "STOP!";                  // Displays STOP.
beepInterval = 120;                // Sets a fast beep rate
lcd.setRGB(255, 0, 0);             // Sets the LDD backlight to red.
}
