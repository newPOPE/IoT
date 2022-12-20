#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <Adafruit_MQTT_FONA.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <ESP8266WiFi.h>
#include "env.h"

#define STATUS_LED LED_BUILTIN

WiFiClient client;
IPAddress ipAddress;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USER, AIO_PASS);

int valTmp = 1;
String channel;

#define SENSORS_COUNT 4               // sensors count
int sensors[] = { 5, 4, 0, 14 };      // GPIO pins
int sensorValues[] = { 0, 0, 0, 0 };  // default values

Adafruit_MQTT_Publish* publisher;

void setup() {
  Serial.begin(9600);
  connectToWifi(WIFI_NAME, WIFI_PASS);

  for (int8_t i = 0; i < SENSORS_COUNT; i++) {
    pinMode(sensors[i], INPUT_PULLUP);
  }

  channel = AIO_CHANNEL_NAME + ipAddress.toString();
  publisher = new Adafruit_MQTT_Publish(&mqtt, channel.c_str());

  pinMode(STATUS_LED, OUTPUT);
}

void blinkStatusLed(uint8 count, boolean pauseAfter = true, uint blinkPause = 200) {
  digitalWrite(STATUS_LED, LOW);
  digitalWrite(BUILTIN_LED, LOW);
  //int blinkPause = 200;  //ms

  for (int i = 0; i < count; i++) {
    digitalWrite(STATUS_LED, HIGH);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(blinkPause);
    digitalWrite(STATUS_LED, LOW);
    digitalWrite(BUILTIN_LED, LOW);
    delay(blinkPause);
  }

  if (pauseAfter) {
    delay(500);
  }
}

void readSensors() {
  for (int i = 0; i < SENSORS_COUNT; i++) {
    Serial.print("sensor #");
    Serial.println(sensors[i]);
    Serial.println("value: " + (String)sensorValues[i]);

    valTmp = digitalRead(sensors[i]);
    boolean changed = false;

    // publish state
    changed = true;
    if (valTmp != sensorValues[i]) {
      Serial.println(valTmp);

      sensorValues[i] = valTmp;
      blinkStatusLed(2, false, 50);
    }

    if (changed) {
      char json[200];
      DynamicJsonDocument jsonDoc(1025);
      for (byte i = 0; i < SENSORS_COUNT; i++) {
        jsonDoc["sensors"][i] = sensorValues[i];
      }
      jsonDoc["state"] = calculateLockState();

      serializeJson(jsonDoc, json);
      publisher->publish(json);
    }
  }
}

char* calculateLockState() {
  // open, closed, locked

  if(sensorValues[0] == 0) {
    return "closed";
  }

  return "open";
}

void loop() {
  MQTT_connect();
  readSensors();

  delay(1000);
}


void connectToWifi(String name, String pass) {
  WiFi.begin(name, pass);
  Serial.print("Connecting to WiFi: ");
  Serial.print(name);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  ipAddress = WiFi.localIP();
  Serial.println("Connected, IP address:" + ipAddress.toString());
  Serial.println("MAC address is: " + WiFi.macAddress());
  Serial.println();
  blinkStatusLed(1);
}

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
    blinkStatusLed(3, false, 100);
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1)
        ;
    }
  }
  Serial.println("MQTT Connected!");
  blinkStatusLed(2);
}