#
set flash_chip s25fl256sxxxxxx0-spi-x1_x2_x4

write_cfgmem -force -format MCS -size 256 -interface SPIx4 -loadbit "up 0x0 ./results/top.bit" -verbose ./results/top.mcs

disconnect_hw_server -quiet
open_hw_manager
connect_hw_server
disconnect_hw_server
connect_hw_server
open_hw_target 
current_hw_device [get_hw_devices]
refresh_hw_device [current_hw_device]

create_hw_bitstream -hw_device [get_hw_devices] [get_property PROGRAM.HW_CFGMEM_BITFILE [current_hw_device]];

#create_hw_cfgmem -hw_device [current_hw_device] -mem_dev [lindex [get_cfgmem_parts {s25fl256sxxxxxx0-spi-x1_x2_x4}] 0]
create_hw_cfgmem -hw_device [current_hw_device] -mem_dev [lindex [get_cfgmem_parts $flash_chip] 0]
set_property PROGRAM.ADDRESS_RANGE  {use_file}              [ get_property PROGRAM.HW_CFGMEM [current_hw_device]]
set_property PROGRAM.FILES [list "./results/top.mcs" ]      [ get_property PROGRAM.HW_CFGMEM [current_hw_device]]
set_property PROGRAM.PRM_FILE {}                            [ get_property PROGRAM.HW_CFGMEM [current_hw_device]]
set_property PROGRAM.UNUSED_PIN_TERMINATION {pull-none}     [ get_property PROGRAM.HW_CFGMEM [current_hw_device]]
set_property PROGRAM.BLANK_CHECK    0                       [ get_property PROGRAM.HW_CFGMEM [current_hw_device]]
set_property PROGRAM.ERASE          1                       [ get_property PROGRAM.HW_CFGMEM [current_hw_device]]
set_property PROGRAM.CFG_PROGRAM    1                       [ get_property PROGRAM.HW_CFGMEM [current_hw_device]]
set_property PROGRAM.VERIFY         1                       [ get_property PROGRAM.HW_CFGMEM [current_hw_device]]
set_property PROGRAM.CHECKSUM       0                       [ get_property PROGRAM.HW_CFGMEM [current_hw_device]]

program_hw_devices  [current_hw_device]; 
refresh_hw_device   [current_hw_device];

program_hw_cfgmem -hw_cfgmem [ get_property PROGRAM.HW_CFGMEM [current_hw_device]]

boot_hw_device -timeout 15  [current_hw_device]
close_hw_manager




#start_gui
#open_hw_manager
#connect_hw_server -allow_non_jtag
#open_hw_target
#disconnect_hw_server localhost:3121
#connect_hw_server -allow_non_jtag
#open_hw_target
#current_hw_device [get_hw_devices xc7a200t_0]
#refresh_hw_device -update_hw_probes false [lindex [get_hw_devices xc7a200t_0] 0]
#create_hw_cfgmem -hw_device [lindex [get_hw_devices xc7a200t_0] 0] [lindex [get_cfgmem_parts {s25fl256sxxxxxx0-spi-x1_x2_x4}] 0]

