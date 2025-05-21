#include <Arduino.h>

#define BUTTON_PIN 4 // GIOP33 pin connected to button
#define LED_PIN 17 // GIOP21 pin connected to LED
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize the pushbutton pin as an pull-up input
  // the pull-up input pin will be HIGH when the switch is open and LOW when the switch is closed.
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // read the state of the switch/button:
  int buttonState = digitalRead(BUTTON_PIN);

  // print out the button's state
  Serial.println(buttonState);

  // if the button is pressed (LOW), turn on the LED
  if (buttonState == LOW) {
    digitalWrite(LED_PIN, LOW);
  } else {
    digitalWrite(LED_PIN, HIGH);
  }
}
