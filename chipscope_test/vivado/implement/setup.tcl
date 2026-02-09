# This script sets up a Vivado project with all ip references resolved.
close_project -quiet
file delete -force proj.xpr *.os *.jou *.log proj.srcs proj.cache proj.runs

#create_project -part  xc7a100tcsg324-1 -force proj 
create_project -part  xc7a200tsbv484-1 -force proj 
set_property target_language verilog [current_project]
set_property default_lib work [current_project]

#read_ip ../source/top_ila/top_ila.xci

#reset_target all [get_files *.xci]
#upgrade_ip -quiet  [get_ips *]
#generate_target {all} [get_ips *]

read_verilog ../../top.v

read_xdc ../source/nexys_top.xdc
#read_xdc ../source/add_ila.xdc  
#set_property used_in_synthesis false [get_files ../source/add_ila.xdc]

close_project




