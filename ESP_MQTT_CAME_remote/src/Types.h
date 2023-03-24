#pragma once

struct sensorState_t {
    float temperature = 0.0;
    float humidity = 0.0;
    float hic = 0.0;
    bool valid = true;
};
