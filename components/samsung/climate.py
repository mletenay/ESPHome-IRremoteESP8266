import esphome.codegen as cg
from esphome.components import climate_ir
from esphome.components import ir_remote_base

AUTO_LOAD = ["climate_ir", "ir_remote_base"]

samsung_ns = cg.esphome_ns.namespace("samsung")
SamsungClimate = samsung_ns.class_("SamsungClimate", climate_ir.ClimateIR)

CONFIG_SCHEMA = climate_ir.climate_ir_with_receiver_schema(SamsungClimate)

async def to_code(config):
    ir_remote_base.load_ir_remote()
    cg.add_define("SEND_SAMSUNG_AC", True)
    await climate_ir.new_climate_ir(config)
