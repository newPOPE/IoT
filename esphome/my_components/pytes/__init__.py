import logging
import esphome.codegen as cg
from esphome.components import uart
import esphome.config_validation as cv
from esphome.const import CONF_ID

_LOGGER = logging.getLogger(__name__)

CODEOWNERS = ["@functionpointer"]
DEPENDENCIES = ["uart"]
MULTI_CONF = True

CONF_PYTES_ID = "pytes_id"
CONF_BATTERY = "battery"

pytes_ns = cg.esphome_ns.namespace("pytes")
PytesComponent = pytes_ns.class_(
    "PytesComponent", cg.PollingComponent, uart.UARTDevice
)
PytesBattery = pytes_ns.class_("PytesBattery")

CV_NUM_BATTERIES = cv.int_range(1, 6)

PYTES_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PYTES_ID): cv.use_id(PytesComponent),
        cv.Required(CONF_BATTERY): CV_NUM_BATTERIES,
    }
)


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(PytesComponent),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
