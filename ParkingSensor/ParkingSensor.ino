
#include <Wire.h>                 // Enables I2C communication with the LCD.
#include "rgb_lcd.h"              // Loads the Grove RGB LCD V5.0 library.

rgb_lcd lcd;                      // Creates the LCD display object.

// Assigns the Arduino pins to the Grove modules.
const int lightPin = A0;          // Reads the Grove light sensor.
const int ledPin = 3;             // Controls the Grove LED module.
const int buzzerPin = 4;          // Controls the Grove buzzer module.
const int buttonPin = 2;          // Reads the Grove button module.

// Stores whether the parking warning system is switched on.
bool systemOn = true;

// Stores the previous button state for detecting a new press.
bool lastButtonState = LOW;

// Records when the button was last pressed.
unsigned long lastPress = 0;

// Records when the buzzer last changed state.
unsigned long lastBeep = 0;

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

  // Creates a variable for the message shown on the LCD.
  const char* status;

  // Checks whether the sensor is receiving plenty of light.
  if (light > 700) {
    status = "SAFE";              // Displays SAFE.
    beepInterval = 0;             // Keeps the buzzer silent.
    lcd.setRGB(0, 255, 0);        // Sets the LCD backlight to green.

  // Checks whether the sensor reading is moderately low.
  } else if (light > 450) {
    status = "CAUTION";           // Displays CAUTION.
    beepInterval = 700;           // Sets a slow beep rate.
    lcd.setRGB(255, 180, 0);      // Sets the LCD backlight to orange.

  // Checks whether the sensor reading is lower still.
  } else if (light > 250) {
    status = "WARNING";           // Displays WARNING.
    beepInterval = 350;           // Sets a faster beep rate.
    lcd.setRGB(255, 60, 0);       // Sets the LCD backlight to orange-red.

  // Runs when the sensor reading is 250 or below.
  } else {
    status = "STOP!";             // Displays STOP!.
    beepInterval = 120;           // Sets a very fast beep rate.
    lcd.setRGB(255, 0, 0);        // Sets the LCD backlight to red.
  }

  // Turns on the LED whenever the system detects a warning.
  digitalWrite(ledPin, beepInterval == 0 ? LOW : HIGH);

  // Reads the current time without stopping the program.
  unsigned long now = millis();

  // Keeps the buzzer off when the sensor reading is in the SAFE zone.
  if (beepInterval == 0) {
    buzzerOn = false;             // Records that the buzzer is off.
    digitalWrite(buzzerPin, LOW); // Switches the buzzer off.

  // Checks whether it is time to change the buzzer state.
  } else if (now - lastBeep >=
             (buzzerOn ? 70UL : (unsigned long)beepInterval)) {

    buzzerOn = !buzzerOn;         // Switches the buzzer on or off.
    digitalWrite(buzzerPin, buzzerOn ? HIGH : LOW); // Applies the new state.
    lastBeep = now;               // Records when the state changed.
  }

  // Moves the LCD cursor to the beginning of the first row.
  lcd.setCursor(0, 0);

  // Prints the sensor reading on the first row.
  lcd.print("Light: ");
  lcd.print(light);

  // Overwrites leftover characters from any previous longer reading.
  lcd.print("     ");

  // Moves the LCD cursor to the beginning of the second row.
  lcd.setCursor(0, 1);

  // Prints the current warning message on the second row.
  lcd.print(status);

  // Fills the rest of the second row to remove leftover characters.
  lcd.print("                ");

  // Prints the sensor reading to the computer for calibration.
  Serial.print("Light level: ");
  Serial.println(light);

  // Waits briefly before taking the next sensor reading.
  delay(50);
}