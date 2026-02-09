
set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 2.5 [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 50 [current_design]
set_property BITSTREAM.Config.SPI_BUSWIDTH 4 [current_design]
set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]

create_clock -name clk100  -period 10.0 [get_ports clkin100]

set_property IOSTANDARD LVCMOS33    [get_ports clkin100]
set_property PACKAGE_PIN E3         [get_ports clkin100]

set_property IOSTANDARD LVCMOS33    [get_ports rstn]
set_property PACKAGE_PIN C2         [get_ports rstn]

set_property IOSTANDARD LVCMOS33 [get_ports {led[*]}]
set_property PACKAGE_PIN T10 [get_ports {led[7]}]
set_property PACKAGE_PIN  T9 [get_ports {led[6]}]
set_property PACKAGE_PIN  J5 [get_ports {led[5]}]
set_property PACKAGE_PIN  H5 [get_ports {led[4]}]
set_property PACKAGE_PIN  H6 [get_ports {led[3]}]
set_property PACKAGE_PIN  J2 [get_ports {led[2]}]
set_property PACKAGE_PIN  J4 [get_ports {led[1]}]
set_property PACKAGE_PIN  F6 [get_ports {led[0]}] 


set_property IOSTANDARD LVCMOS33 [get_ports {qspi_*}]
set_property PACKAGE_PIN M14 [get_ports {qspi_io_io[3]}]
set_property PACKAGE_PIN L14 [get_ports {qspi_io_io[2]}]
set_property PACKAGE_PIN K18 [get_ports {qspi_io_io[1]}]
set_property PACKAGE_PIN K17 [get_ports {qspi_io_io[0]}]
set_property PACKAGE_PIN L13 [get_ports {qspi_ss_io}]
#set_property PACKAGE_PIN  E9 [get_ports {qspi_sck_io}]
#set_property PACKAGE_PIN L16 [get_ports {qspi_sck_io}]

set_property IOSTANDARD LVCMOS33 [get_ports {uart_*}]
set_property PACKAGE_PIN D10 [get_ports {uart_txd}]
set_property PACKAGE_PIN  A9 [get_ports {uart_rxd}]
