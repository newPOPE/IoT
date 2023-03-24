#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <LiquidCrystal_I2C.h>
#include "env.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT);
Adafruit_MQTT_Subscribe statusSubsription = Adafruit_MQTT_Subscribe(&mqtt, AIO_CHANNEL_NAME_STATUS);
Adafruit_MQTT_Subscribe batterySubsription = Adafruit_MQTT_Subscribe(&mqtt, AIO_CHANNEL_NAME_BATTERY);

StaticJsonDocument<200> statusDocument;
StaticJsonDocument<200> batteryDocument;

void setup() {
  Serial.begin(9600);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to ");
  lcd.setCursor(0, 1);
  lcd.print(WLAN_SSID);

  WiFi.hostname("FVE stats");
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  mqtt.subscribe(&statusSubsription);
  statusSubsription.setCallback(storeStatusData);
  mqtt.subscribe(&batterySubsription);
  batterySubsription.setCallback(storeBatteryData);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  MQTT_connect();
  digitalWrite(LED_BUILTIN, 1);
  //mqtt.processPackets(1000);
  delay(1000);
  digitalWrite(LED_BUILTIN, 0);
  //Serial.println("loop");

  if (!mqtt.ping()) {
    mqtt.disconnect();
  }
}

void storeStatusData(char *data, uint16_t l) {
  Serial.println(data);
  deserializeJson(statusDocument, data);

  printStatsToDisplay();
}

void storeBatteryData(char *data, uint16_t l) {
  Serial.println(data);
  deserializeJson(batteryDocument, data);

  printStatsToDisplay();
}

void printStatsToDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S: " + statusDocument["Status"].as<String>());
  lcd.setCursor(0, 1);
  lcd.print("Batt: ");
  lcd.print(batteryDocument["BatterySOC"].as<String>() + "% | ");
  lcd.print(batteryDocument["Bat_pwrDir"].as<String>() + "W");
  lcd.setCursor(0, 2);
  lcd.print("PV: " + statusDocument["Ppv1L"].as<String>() + "W | ");
  lcd.print(statusDocument["Epv1_todayL"].as<String>() + "kWh");
  lcd.setCursor(0, 3);
  lcd.print("Load: " + statusDocument["OP_WattL"].as<String>() + "W");
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {  // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1)
        ;
    }
  }
  Serial.println("MQTT Connected!");
}