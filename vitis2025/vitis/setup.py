# vitis -s setup.py
# vitis 
#   then do file -> "set workspace" or file -> "open recent workspace"

import vitis
import os
import sys

app_name = sys.argv[1]
print("app_name = ", app_name)


#app_name = "hello_world"
#app_name = "bram_test"

plat_name = "standalone_plat"
hw_xsa = "../implement/results/top.xsa"

os.system('rm -rf workspace')

client = vitis.create_client()
client.set_workspace(path="workspace")

advanced_options = client.create_advanced_options_dict(dt_overlay="0")

platform = client.create_platform_component(
    name = plat_name,
    hw_design = hw_xsa,
    os = "standalone",
    cpu = "microblaze_0",
    domain_name = "standalone_microblaze_0",
    generate_dtb = False,
    advanced_options = advanced_options,
    compiler = "gcc"
)

platform = client.get_component(name=plat_name)

status = platform.build()

comp = client.create_app_component(
    name=app_name,
    #platform = "$COMPONENT_LOCATION/../standalone_plat/export/standalone_plat/standalone_plat.xpfm",
    platform = "./workspace/standalone_plat/export/standalone_plat/standalone_plat.xpfm",
    domain = "standalone_microblaze_0",
    template = "empty_application"
)

os.system(f"ln -s ../../../src/{app_name}/{app_name}.c workspace/{app_name}/src/{app_name}.c")
os.system(f'ln -s ../../../src/fpga.h workspace/{app_name}/src/fpga.h')

status = comp.clean()
status = platform.build()
comp.build()
vitis.dispose()

