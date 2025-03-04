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

read_ip ../source/top_ila/top_ila.xci

upgrade_ip -quiet  [get_ips *]
generate_target {all} [get_ips *]

source ../source/system1.tcl
generate_target {synthesis implementation} [get_files ./proj.srcs/sources_1/bd/system1/system1.bd]
set_property synth_checkpoint_mode None [get_files ./proj.srcs/sources_1/bd/system1/system1.bd]

source ../source/system2.tcl
generate_target {synthesis implementation} [get_files ./proj.srcs/sources_1/bd/system2/system2.bd]
set_property synth_checkpoint_mode None [get_files ./proj.srcs/sources_1/bd/system2/system2.bd]

read_verilog -sv ../source/top.sv

read_xdc         ../source/top.xdc

add_files -norecurse ../vitis1/release/production1.elf
set_property SCOPED_TO_REF system1 [get_files -all -of_objects [get_fileset sources_1] {production1.elf}]
set_property SCOPED_TO_CELLS { microblaze_0 } [get_files -all -of_objects [get_fileset sources_1] {production1.elf}]

add_files -norecurse ../vitis2/release/production2.elf
set_property SCOPED_TO_REF system2 [get_files -all -of_objects [get_fileset sources_1] {production2.elf}]
set_property SCOPED_TO_CELLS { microblaze_1 } [get_files -all -of_objects [get_fileset sources_1] {production2.elf}]

close_project

