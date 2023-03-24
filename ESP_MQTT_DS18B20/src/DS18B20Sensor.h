#include "Types.h"

#include <OneWire.h>
#include <DS18B20.h>

#define ONE_WIRE_BUS            14

OneWire oneWire(ONE_WIRE_BUS);
DS18B20 sensor(&oneWire);


namespace DS18B20Sensor {

    void setup () {
        if (sensor.begin() == false) {
            Serial.println("Error: Could not find sensor.");
            while (1);
        }

        sensor.setResolution(12);
    }

    void handle(sensorState_t &state) {
        sensor.requestTemperatures();

  
        while (!sensor.isConversionComplete());

        state.temperature = sensor.getTempC();
        state.valid = true;
    }
}