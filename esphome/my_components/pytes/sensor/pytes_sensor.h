#pragma once

#include "../pytes.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace pytes {

class PytesSensor : public PytesListener, public Component {
 public:
  PytesSensor(int8_t bat_num);
  void dump_config() override;

  SUB_SENSOR(voltage)
  SUB_SENSOR(current)
  SUB_SENSOR(temperature)
  SUB_SENSOR(temperature_low)
  SUB_SENSOR(temperature_high)
  SUB_SENSOR(voltage_low)
  SUB_SENSOR(voltage_high)

  SUB_SENSOR(coulomb)

  void on_line_read(LineContents *line) override;

 protected:
  int8_t bat_num_;
};

}  // namespace pytes
}  // namespace esphome
