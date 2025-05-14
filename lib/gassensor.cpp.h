#include <Arduino.h>

int smokeA0 = 4;
// Your threshold value
int sensorThres = 700;
int ledPin = 2;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(smokeA0, INPUT);
  Serial.begin(9600);
}

void loop() {
  int analogSensor = analogRead(smokeA0);

  Serial.print("Pin A0: ");
  Serial.println(analogSensor);

  if (analogSensor > sensorThres) {
    Serial.println("Højt røgtniveau, advarselslampe blinker");
    digitalWrite(ledPin, HIGH);
    delay(250);                  // Blink funktion, on.
    digitalWrite(ledPin, LOW);
    delay(250);                  // Blink funktion, off.
  }

  else{ 
    digitalWrite(ledPin, LOW);
    delay(500);
  }



  }
