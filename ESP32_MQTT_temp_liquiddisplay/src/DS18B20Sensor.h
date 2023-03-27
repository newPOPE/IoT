#include <OneWire.h>
#include <DS18B20.h>

#define ONE_WIRE_BUS              15

OneWire oneWire(ONE_WIRE_BUS);
DS18B20 sensor(&oneWire);

struct sensorState_t {

    float temperature = 0.0;
    bool valid = false;
};


namespace DS18B20Sensor {

    void setup () {

        sensor.begin();
    }

    void handle (sensorState_t &state) {

        sensor.requestTemperatures();

        while (!sensor.isConversionComplete());  // wait until sensor is ready

        Serial.print("Temp: ");
        Serial.println(sensor.getTempC());

        state.temperature = sensor.getTempC();
        state.valid = true;

    }
}