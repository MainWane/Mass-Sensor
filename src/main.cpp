#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// I2C-adresse: oftest 0x27 eller 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Wire.begin();  // Starter I2C-bussen
  lcd.init();   // Initialiser LCD
  lcd.backlight(); // Tænd baggrundslys

  lcd.setCursor(0, 0);
  lcd.print("Hej fra ESP32!");
  lcd.setCursor(0, 1);
  lcd.print("Laus er en skumfidus :)");
}

void loop() {
  // LCD viser tekst konstant - intet i loop
}
