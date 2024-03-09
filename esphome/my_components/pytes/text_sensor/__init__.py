import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import CONF_ID

from .. import (
    CONF_PYTES_ID,
    PYTES_COMPONENT_SCHEMA,
    CONF_BATTERY,
    pytes_ns,
)

PytesTextSensor = pytes_ns.class_("PytesTextSensor", cg.Component)

CONF_BASE_STATE = "base_state"
CONF_VOLTAGE_STATE = "voltage_state"
CONF_CURRENT_STATE = "current_state"
CONF_TEMPERATURE_STATE = "temperature_state"

MARKERS: list[str] = [
    CONF_BASE_STATE,
    CONF_VOLTAGE_STATE,
    CONF_CURRENT_STATE,
    CONF_TEMPERATURE_STATE,
]

CONFIG_SCHEMA = PYTES_COMPONENT_SCHEMA.extend(
    {cv.GenerateID(): cv.declare_id(PytesTextSensor)}
).extend({cv.Optional(marker): text_sensor.text_sensor_schema() for marker in MARKERS})


async def to_code(config):
    paren = await cg.get_variable(config[CONF_PYTES_ID])
    bat = cg.new_Pvariable(config[CONF_ID], config[CONF_BATTERY])

    for marker in MARKERS:
        if marker_config := config.get(marker):
            var = await text_sensor.new_text_sensor(marker_config)
            cg.add(getattr(bat, f"set_{marker}_text_sensor")(var))

    cg.add(paren.register_listener(bat))
