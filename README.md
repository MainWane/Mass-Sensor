***Mass-Sensor

Dette repository indeholder kode til en række forskellige sensorer, som kan måle alt fra strøm og spænding til temperatur, fugt og luftkvalitet. Koden er skrevet til Arduino/PlatformIO og kan nemt integreres i egne projekter.

**Indhold

src/ – Hovedkoden til de forskellige sensorer

lib/ – Eksterne biblioteker og drivere

include/ – Header-filer

test/ – Eksempler på brug af sensorer

platformio.ini – PlatformIO konfigurationsfil

**Understøttede sensorer

Dette repository indeholder fungerende kode til følgende sensorer:

INA219 – Strøm- og spændingsmåling via I²C

BMP280 – Tryk- og temperaturmåling via I²C

BME280 – Tryk, temperatur og luftfugtighed via I²C

DHT11 – Temperatur og luftfugtighed via digital pin

Dallas temp sensor (DS18B20) – Digitale temperaturmålinger via 1-Wire

MQ135 – Luftkvalitet/gasser

ACS712 – Strømsensor via analog input

**Installation

1. Klon repository:
```
 git clone https://github.com/MainWane/Mass-Sensor.git
```
2. Åbn platformio.ini i PlatformIO IDE.

3. Installer de nødvendige biblioteker via PlatformIO Library Manager.
