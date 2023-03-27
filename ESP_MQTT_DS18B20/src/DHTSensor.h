#pragma once

#include "DHT.h"
#include "Types.h"

#define DHTPIN 0  // Digital pin connected to the DHT sensor
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
// #define DHTTYPE DHT22  // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

namespace DHTSensor {
  void setup() {
    dht.begin();
  }

  void handle(sensorState_t& state) {
    delay(1000);

    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      //state.valid = false;
      return;
    }

    float hic = dht.computeHeatIndex(t, h, false);
    //float correction = -2.0; // DHT22 from ALI
    //float correction = -1.0;

    state.temperatureDht = t;
    state.humidityDht = h;
  }
}