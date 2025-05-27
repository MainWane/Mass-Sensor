// Kode mangler tilføjelser
// Sender data fra Ina219 til subscriber på raspberry (og ikke andet)

#include <Wire.h>
#include <Adafruit_INA219.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi og MQTT-indstillinger
#define WIFI_SSID "MyPIAP"
#define WIFI_PASSWORD "mypassword123"
#define MQTT_SERVER "10.42.0.1"
#define MQTT_PORT 1883

WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_INA219 ina219;

#define BUTTON_PIN 4
#define GREEN_LED_PIN 33
#define YELLOW_LED_PIN 35
#define RED_LED_PIN 32

const float OVERCURRENT_THRESHOLD = 0.7; // A

bool manualRelayState = true; // starter tændt
bool emergencyShutdown = false;
const int relayPin = 27; // GPIO pin for relæet
const unsigned long sampleInterval = 1000; // måling hvert sekund
const unsigned long printInterval = 10000; // udskriv hvert 10. sekund
const int avgSamples = 10; // antal samples til gennemsnit
unsigned long lastSampleTime = 0;
unsigned long lastPrintTime = 0;
float energy_Wh = 0.0;
float voltageSum = 0.0;
float currentSum = 0.0;
int sampleCount = 0;
float lastAvgPower = 0.0;

void setup_wifi() {
    Serial.print("Forbinder til WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        }
    Serial.println("\nWiFi forbundet.");
    }

void reconnect_mqtt() {
    while (!client.connected()) {
            Serial.print("Forbinder til MQTT...");
            if (client.connect("INA219Client")) {
            Serial.println(" tilsluttet.");
            } else {
            Serial.print(" fejl, rc=");
            Serial.print(client.state());
            Serial.println(" prøver igen om 5 sek.");
            delay(5000);
            }
            }
}

void setup() {

pinMode(relayPin, OUTPUT);
pinMode(BUTTON_PIN, INPUT_PULLDOWN); // kræver ekstern eller intern pulldown (hvis ESP32 understøtter)
pinMode(GREEN_LED_PIN, OUTPUT);
pinMode(YELLOW_LED_PIN, OUTPUT);
pinMode(RED_LED_PIN, OUTPUT);

// Starttilstande
digitalWrite(relayPin, HIGH);
digitalWrite(GREEN_LED_PIN, HIGH);
digitalWrite(YELLOW_LED_PIN, LOW);
digitalWrite(RED_LED_PIN, LOW);

    Serial.begin(9600);
    Wire.begin();
    
    if (!ina219.begin()) {
    Serial.println("INA219 ikke fundet. Tjek forbindelser.");
    
    while (1);
    }
    pinMode(relayPin, OUTPUT);
    // --- Knapstyring ---
if (!emergencyShutdown) {
  if (digitalRead(BUTTON_PIN) == HIGH) {
    manualRelayState = !manualRelayState;
    delay(200); // simpelt debounce
  }
  digitalWrite(relayPin, manualRelayState ? HIGH : LOW);
} else {
  digitalWrite(relayPin, LOW); // nødstop = sluk relæ
}

// --- LED-Styring ---

// Grøn LED: relæ aktivt
digitalWrite(GREEN_LED_PIN, (!emergencyShutdown && manualRelayState) ? HIGH : LOW);

// Gul LED: høj strøm, kun hvis ikke nødstop
digitalWrite(YELLOW_LED_PIN, (!emergencyShutdown && lastAvgPower / ina219.getBusVoltage_V() > OVERCURRENT_THRESHOLD) ? HIGH : LOW);

// Rød LED: nødstop aktivt
digitalWrite(RED_LED_PIN, emergencyShutdown ? HIGH : LOW);

}

void loop() {
    if (!client.connected()) reconnect_mqtt();
    client.loop();
    unsigned long currentMillis = millis();

    if (currentMillis - lastSampleTime >= sampleInterval) {
    lastSampleTime = currentMillis;
    float voltage = ina219.getBusVoltage_V();
    float current = ina219.getCurrent_mA() / 1000.0; // A
    voltageSum += voltage;
    currentSum += current;
    sampleCount++;

    if (sampleCount >= avgSamples) {
    float avgVoltage = voltageSum / sampleCount;
    float avgCurrent = currentSum / sampleCount;
    lastAvgPower = avgVoltage * avgCurrent;
    float periodHours = (sampleInterval * avgSamples) / 3600000.0;
    energy_Wh += lastAvgPower * periodHours;
    voltageSum = 0.0;
    currentSum = 0.0;
    sampleCount = 0;

    // MQTT publish
    char buf[16];
    dtostrf(avgVoltage, 1, 2, buf);
    client.publish("esp32/ina219/voltage", buf);
    dtostrf(avgCurrent, 1, 3, buf);
    client.publish("esp32/ina219/current", buf);
    dtostrf(lastAvgPower, 1, 3, buf);
    client.publish("esp32/ina219/power", buf);
    dtostrf(energy_Wh, 1, 3, buf);
    client.publish("esp32/ina219/energy_wh", buf);
    }
    }
    
    if (currentMillis - lastPrintTime >= printInterval) {
    lastPrintTime = currentMillis;
    Serial.print("Effekt (W): ");
    Serial.print(lastAvgPower, 4);
    Serial.print(" | Energi (Wh): ");
    Serial.println(energy_Wh, 4);
    }
    // RELÆ ER ALTID TÆNDT
    digitalWrite(relayPin, HIGH);
}