# Script to compile the FPGA with zynq processor system all the way to bit file.

close_project -quiet

open_project proj.xpr

update_compile_order -fileset sources_1
reset_run synth_1
launch_runs synth_1 -jobs 8
wait_on_run synth_1

launch_runs impl_1 -jobs 8
wait_on_run impl_1

open_run impl_1
report_timing_summary   -file   ./results/timing.rpt
report_utilization      -file   ./results/utilization.rpt

# multiboot production bitstream
set_property BITSTREAM.CONFIG.CONFIGFALLBACK ENABLE [current_design]
write_bitstream -verbose -force ./results/top2.bit

# multiboot golden bitstream
set_property BITSTREAM.CONFIG.CONFIGFALLBACK ENABLE [current_design]
set_property BITSTREAM.CONFIG.NEXT_CONFIG_ADDR 0x03F0000 [current_design] ;# points to timer1.bin start address
write_bitstream -verbose -force ./results/top1.bit

close_project


