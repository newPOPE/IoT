#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <LiquidCrystal_I2C.h>
#include "env.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT);
Adafruit_MQTT_Subscribe mqttSubscription = Adafruit_MQTT_Subscribe(&mqtt, AIO_CHANNEL_NAME);

StaticJsonDocument<800> document;
StaticJsonDocument<400> jsonFilter;

void setup() {
  Serial.begin(9600);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

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

  
  lcd.init();
  lcd.backlight();

  mqtt.subscribe(&mqttSubscription);
  mqttSubscription.setCallback(storeData);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  MQTT_connect();
  digitalWrite(LED_BUILTIN, 1);
  //mqtt.processPackets(1000);
  digitalWrite(LED_BUILTIN, 0);
  //Serial.println("loop");

  if (!mqtt.ping()) {
    mqtt.disconnect();
  }

  printStatsToDisplay(2000);
}

void storeData(char *data, uint16_t l) {
  jsonFilter["Epv1_todayL"] = true;
  jsonFilter["BatterySOC"] = true;
  jsonFilter["Ppv1L"] = true;
  jsonFilter["OP_WattL"] = true;
  jsonFilter["Status"] = true;

  
  deserializeJson(document, data, DeserializationOption::Filter(jsonFilter));
  
  
}

void printStatsToDisplay(int pauseInMs) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S:" + document["Status"].as<String>());
  delay(pauseInMs);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pc:" + document["Epv1_todayL"].as<String>() + "kW ");
  lcd.print("Bs:" + document["BatterySOC"].as<String>() + "%");
  lcd.setCursor(0, 1);
  lcd.print("Pv:" + document["Ppv1L"].as<String>() + "W ");
  lcd.print("Po:" + document["OP_WattL"].as<String>() + "W");

  delay(pauseInMs);
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