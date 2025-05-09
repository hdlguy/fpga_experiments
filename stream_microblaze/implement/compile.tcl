# Script to compile the FPGA with zynq processor system all the way to bit file.

close_project -quiet

open_project proj.xpr

update_compile_order -fileset sources_1
reset_run synth_1
launch_runs synth_1 -jobs 8
wait_on_run synth_1

#source add_ila.tcl

launch_runs impl_1 -jobs 8
wait_on_run impl_1

open_run impl_1
report_timing_summary   -file   ./results/timing.rpt
report_utilization      -file   ./results/utilization.rpt
write_debug_probes      -force  ./results/top.ltx
write_mem_info          -force  ./results/top.mmi
write_hw_platform -fixed -force -file   ./results/top.xsa

#set_property CFGBVS VCCO [current_design]
#set_property CONFIG_VOLTAGE 2.5 [current_design]
#set_property BITSTREAM.CONFIG.CONFIGRATE 50 [current_design]
#set_property BITSTREAM.Config.SPI_BUSWIDTH 4 [current_design]
#set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]

write_bitstream -verbose -force -bin_file ./results/top.bit

close_project


