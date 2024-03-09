#include "pytes_text_sensor.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pytes {

static const char *const TAG = "pytes.textsensor";

PytesTextSensor::PytesTextSensor(int8_t bat_num) { this->bat_num_ = bat_num; }

void PytesTextSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Pytes Text Sensor:");
  ESP_LOGCONFIG(TAG, " Battery %d", this->bat_num_);
  LOG_TEXT_SENSOR("  ", "Base state", this->base_state_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Voltage state", this->voltage_state_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Current state", this->current_state_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Temperature state", this->temperature_state_text_sensor_);
}

void PytesTextSensor::on_line_read(PytesListener::LineContents *line) {
  if (this->bat_num_ != line->bat_num) {
    return;
  }
  if (this->base_state_text_sensor_ != nullptr) {
    this->base_state_text_sensor_->publish_state(std::string(line->base_st));
  }
  if (this->voltage_state_text_sensor_ != nullptr) {
    this->voltage_state_text_sensor_->publish_state(std::string(line->volt_st));
  }
  if (this->current_state_text_sensor_ != nullptr) {
    this->current_state_text_sensor_->publish_state(std::string(line->curr_st));
  }
  if (this->temperature_state_text_sensor_ != nullptr) {
    this->temperature_state_text_sensor_->publish_state(std::string(line->temp_st));
  }
}

}  // namespace pytes
}  // namespace esphome
