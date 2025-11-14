# This script sets up a Vivado project with all ip references resolved.
close_project -quiet
file delete -force proj.xpr *.os *.jou *.log proj.srcs proj.cache proj.runs
#
create_project -part xc7a100tcsg324-2 -force proj
set_property target_language Verilog [current_project]
set_property default_lib work [current_project]
#load_features ipintegrator

read_ip ../source/interpx5/interpx5.xci
upgrade_ip -quiet  [get_ips *]
generate_target {all} [get_ips *]

read_verilog -sv ../source/interpx5_tb.sv

add_files -fileset sim_1 -norecurse ./interpx5_tb_behav.wcfg

close_project

#########################



