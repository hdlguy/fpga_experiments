# vitis -s setup.py hello1
# vitis --workspace ./workspace/

import vitis
import os
import sys

#app_name = sys.argv[1]
app_name = "hello1"
print("app_name = ", app_name)

plat_name = "standalone_plat"
domain_name = "standalone_domain"
hw_xsa = "../implement/results/top.xsa"
cpu_name = "microblaze_0"
#cpu_name = "ps7_cortexa9_0"
#cpu_name = "psu_cortexa53_0"

os.system('rm -rf workspace')

client = vitis.create_client()
client.set_workspace(path="workspace")

advanced_options = client.create_advanced_options_dict(dt_overlay="0")

platform = client.create_platform_component(
    name = plat_name,
    hw_design = hw_xsa,
    os = "standalone",
    cpu = cpu_name,
    domain_name = domain_name,
    generate_dtb = False,
    advanced_options = advanced_options,
    compiler = "gcc"
)

#status = platform.standalone_domain.set_config(option = "lib", param = "XILTIMER_sleep_timer", value = "axi_timer_0", lib_name="xiltimer")

#platform = client.get_component(name=plat_name)

status = platform.build()

# create empty application
comp = client.create_app_component(
    name=app_name,
    platform = "./workspace/standalone_plat/export/standalone_plat/standalone_plat.xpfm",
    domain = domain_name,
    template = "empty_application"
)

# add source files as symbolic links
os.system(f"ln -s ../../../src/{app_name}/test.c workspace/{app_name}/src/test.c")
os.system(f'ln -s ../../../src/fpga.h workspace/{app_name}/src/fpga.h')

status = comp.clean()
status = platform.build()
comp.build()
vitis.dispose()

