#include "Types.h"

#include <OneWire.h>
#include <DS18B20.h>


#define SENSOR_1_PIN            14
#define SENSOR_2_PIN            12

OneWire oneWire(SENSOR_1_PIN);
OneWire oneWire2(SENSOR_2_PIN);
DS18B20 sensor(&oneWire);
DS18B20 sensor2(&oneWire2);


namespace DS18B20Sensor {

    void setup () {
        if (sensor.begin() == false) {
            Serial.println("Error: Could not find sensor 1.");
            
            while (1);
        }

        if (sensor2.begin() == false) {
            Serial.println("Error: Could not find sensor 2.");
            
            while (1);
        }

        sensor.setResolution(12);
        sensor2.setResolution(12);
    }

    void handle(sensorState_t &state) {
        sensor.requestTemperatures();
        sensor2.requestTemperatures();

  
        while (!sensor.isConversionComplete());
        while (!sensor2.isConversionComplete());

        state.temperature1 = sensor.getTempC();
        state.temperature11 = state.temperature1 + 0.5;
        state.temperature2 = sensor2.getTempC();

        state.valid = true;
    }
}