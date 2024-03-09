#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace pytes {

static const uint8_t NUM_BUFFERS = 20;
static const uint8_t TEXT_SENSOR_MAX_LEN = 8;

class PytesListener {
 public:
  struct LineContents {
    int bat_num = 0, volt, curr, tempr, tlow, thigh, vlow, vhigh, coulomb, mostempr;
    char base_st[TEXT_SENSOR_MAX_LEN], volt_st[TEXT_SENSOR_MAX_LEN], curr_st[TEXT_SENSOR_MAX_LEN],
        temp_st[TEXT_SENSOR_MAX_LEN];
  };

  virtual void on_line_read(LineContents *line);
  virtual void dump_config();
};

class PytesComponent : public PollingComponent, public uart::UARTDevice {
 public:
  PytesComponent();

  /// Schedule data readings.
  void update() override;
  /// Read data once available
  void loop() override;
  /// Setup the sensor and test for a connection.
  void setup() override;
  void dump_config() override;

  float get_setup_priority() const override;

  void register_listener(PytesListener *listener) { this->listeners_.push_back(listener); }

 protected:
  void process_line_(std::string &buffer);

  // ring buffer
  std::string buffer_[NUM_BUFFERS];
  int buffer_index_write_ = 0;
  int buffer_index_read_ = 0;

  std::vector<PytesListener *> listeners_{};
};

}  // namespace pytes
}  // namespace esphome
