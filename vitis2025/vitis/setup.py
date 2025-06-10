# 2025-06-10T10:46:10.795407
import vitis

client = vitis.create_client()
client.set_workspace(path="vitis")

advanced_options = client.create_advanced_options_dict(dt_overlay="0")

platform = client.create_platform_component(name = "standalone_plat",hw_design = "$COMPONENT_LOCATION/../../implement/results/top.xsa",os = "standalone",cpu = "microblaze_0",domain_name = "standalone_microblaze_0",generate_dtb = False,advanced_options = advanced_options,compiler = "gcc")

platform = client.get_component(name="standalone_plat")
status = platform.build()

comp = client.create_app_component(name="hello_world",platform = "$COMPONENT_LOCATION/../standalone_plat/export/standalone_plat/standalone_plat.xpfm",domain = "standalone_microblaze_0",template = "hello_world")

comp = client.get_component(name="hello_world")
status = comp.clean()

status = platform.build()

comp.build()

vitis.dispose()

