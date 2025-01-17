
set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 2.5 [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 6 [current_design]
set_property BITSTREAM.Config.SPI_BUSWIDTH 4 [current_design]
set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]

create_clock -name clk100  -period 10.0 [get_ports clkin100]

set_property IOSTANDARD LVCMOS25    [get_ports {clkin100}]; # Vadj is 2.5V
set_property PACKAGE_PIN F4         [get_ports {clkin100}]

set_property IOSTANDARD LVCMOS33    [get_ports {uart_*}]
set_property PACKAGE_PIN A18        [get_ports {uart_rxd}]
set_property PACKAGE_PIN B18        [get_ports {uart_txd}]

set_property IOSTANDARD LVCMOS33 [get_ports {led[*]}]
set_property PACKAGE_PIN L15 [get_ports {led[7]}]
set_property PACKAGE_PIN L16 [get_ports {led[6]}]
set_property PACKAGE_PIN L18 [get_ports {led[5]}]
set_property PACKAGE_PIN M18 [get_ports {led[4]}]

set_property PACKAGE_PIN V11 [get_ports {led[3]}]
set_property PACKAGE_PIN V10 [get_ports {led[2]}]
set_property PACKAGE_PIN T11 [get_ports {led[1]}]
set_property PACKAGE_PIN  T9 [get_ports {led[0]}] 


set_property IOSTANDARD LVCMOS33 [get_ports {qspi_*}]
set_property PACKAGE_PIN M14 [get_ports {qspi_io_io[3]}]
set_property PACKAGE_PIN L14 [get_ports {qspi_io_io[2]}]
set_property PACKAGE_PIN K18 [get_ports {qspi_io_io[1]}]
set_property PACKAGE_PIN K17 [get_ports {qspi_io_io[0]}]
set_property PACKAGE_PIN L13 [get_ports {qspi_ss_io}]
#set_property PACKAGE_PIN xxx [get_ports {qspi_sck_io}] ;# not needed because we use CCLK dedicated pin through startup block

