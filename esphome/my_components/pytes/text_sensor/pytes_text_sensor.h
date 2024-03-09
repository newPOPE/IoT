#pragma once

#include "../pytes.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace pytes {

class PytesTextSensor : public PytesListener, public Component {
 public:
  PytesTextSensor(int8_t bat_num);
  void dump_config() override;

  SUB_TEXT_SENSOR(base_state)
  SUB_TEXT_SENSOR(voltage_state)
  SUB_TEXT_SENSOR(current_state)
  SUB_TEXT_SENSOR(temperature_state)

  void on_line_read(LineContents *line) override;

 protected:
  int8_t bat_num_;
};

}  // namespace pytes
}  // namespace esphome
