#pragma once

struct sensorState_t {
    float temperature1 = 0.0;
    float temperature11 = 0.0;
    float temperature2 = 0.0;
    float temperatureDht = 0.0;
    float humidityDht = 0.0;
    bool valid = false;
};
