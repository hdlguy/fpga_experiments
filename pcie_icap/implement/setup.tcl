# This script sets up a Vivado project with all ip references resolved.
close_project -quiet
file delete -force proj.xpr *.os *.jou *.log proj.srcs proj.cache proj.runs
#
create_project -part xc7a100tfgg484-2 -force proj 
set_property target_language Verilog [current_project]
set_property default_lib work [current_project]
load_features ipintegrator

source ../source/top_ila.tcl

source ../source/system.tcl
generate_target {synthesis implementation} [get_files ./proj.srcs/sources_1/bd/system/system.bd]
set_property synth_checkpoint_mode None    [get_files ./proj.srcs/sources_1/bd/system/system.bd]

read_verilog -sv ../source/mem_regfile/mem_regfile.sv

read_verilog -sv ../source/top.sv

read_xdc ../source/top.xdc

close_project

#########################


# system2_microblaze_0_0

