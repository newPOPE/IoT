#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <LiquidCrystal_I2C.h>
#include "env.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);


/************************* Adafruit.io Setup *********************************/

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT);
Adafruit_MQTT_Subscribe mqttSubscription = Adafruit_MQTT_Subscribe(&mqtt, AIO_CHANNEL_NAME);

StaticJsonDocument<200> jsonFilter;
StaticJsonDocument<400> doc;

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();

  mqtt.subscribe(&mqttSubscription);


  jsonFilter["Epv1_todayL"] = true;
  jsonFilter["BatterySOC"] = true;
  jsonFilter["Ppv1L"] = true;
  jsonFilter["OP_WattL"] = true;
}

void loop() {
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &mqttSubscription) {
      Serial.print(F("Got: "));
      deserializeJson(doc, (char *)mqttSubscription.lastread, DeserializationOption::Filter(jsonFilter));
      //serializeJsonPretty(doc, Serial);
    }
  }

  if (!mqtt.ping()) {
    mqtt.disconnect();
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pc:" + doc["Epv1_todayL"].as<String>() + "kW ");
  lcd.print("Bs:" + doc["BatterySOC"].as<String>() + "%");
  lcd.setCursor(0, 1);
  lcd.print("Pv:" + doc["Ppv1L"].as<String>() + "W ");
  lcd.print("Po:" + doc["OP_WattL"].as<String>() + "W");

  delay(1000);
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