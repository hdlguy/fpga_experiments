# This script sets up a Vivado project with all ip references resolved.
close_project -quiet
file delete -force proj.xpr *.os *.jou *.log proj.srcs proj.cache proj.runs
#
create_project -force proj 
set_property part xc7a35ticsg324-1L [current_project]
set_property target_language verilog [current_project]
set_property default_lib work [current_project]

#read_ip ../source/top_ila/top_ila.xci

#upgrade_ip -quiet  [get_ips *]

read_verilog -sv ../datagen.sv
read_verilog -sv ../datagen_tb.sv

add_files -fileset sim_1 -norecurse ./datagen_tb_behav.wcfg

close_project


