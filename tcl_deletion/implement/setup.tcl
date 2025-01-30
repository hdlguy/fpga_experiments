# This script sets up a Vivado project with all ip references resolved.
# run on linux command line with:
#       vivado -mode batch -source setup.tcl
#
close_project -quiet
file delete -force proj.xpr *.os *.jou *.log proj.srcs proj.cache proj.runs

create_project -part xc7a100tcsg324-1 -force proj 
set_property target_language verilog [current_project]
set_property default_lib work [current_project]
load_features ipintegrator
tclapp::install ultrafast -quiet

#read_ip ../source/clkwiz/clkwiz.xci
#read_ip ../source/top_ila/top_ila.xci

#upgrade_ip -quiet  [get_ips *]
#generate_target {all} [get_ips *]

read_verilog -sv ../source/top.sv

read_xdc         ../source/arty_top.xdc

close_project

