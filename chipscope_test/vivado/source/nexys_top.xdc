
set_property CONFIG_VOLTAGE 3.3 [current_design]
set_property CFGBVS VCCO [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 6   [current_design]
set_property BITSTREAM.Config.SPI_BUSWIDTH 4 [current_design]
set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]

create_clock -name clk100  -period 10.0 [get_ports clkin100]

set_property IOSTANDARD LVCMOS33 [get_ports clkin100]
set_property PACKAGE_PIN R4 [get_ports clkin100]

set_property IOSTANDARD LVCMOS25 [get_ports {led[*]}]
set_property PACKAGE_PIN Y13 [get_ports {led[7]}]
set_property PACKAGE_PIN W15 [get_ports {led[6]}]
set_property PACKAGE_PIN W16 [get_ports {led[5]}]
set_property PACKAGE_PIN V15 [get_ports {led[4]}]
set_property PACKAGE_PIN U16 [get_ports {led[3]}]
set_property PACKAGE_PIN T16 [get_ports {led[2]}]
set_property PACKAGE_PIN T15 [get_ports {led[1]}]
set_property PACKAGE_PIN T14 [get_ports {led[0]}]


