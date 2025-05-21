#include <Wire.h>
#include <Arduino.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial); // vent på serial
  Serial.println("\nScanning...");

  for (byte addr = 1; addr < 127; ++addr) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found I2C device at 0x");
      Serial.println(addr, HEX);
    }
  }
}

void loop() {}
