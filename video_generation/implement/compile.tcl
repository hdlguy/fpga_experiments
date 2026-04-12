# Script to compile the FPGA all the way to bit file.
close_project -quiet
file delete -force results
file mkdir ./results

open_project proj.xpr
update_compile_order -fileset sources_1

reset_run synth_1
launch_runs synth_1 -jobs 4
wait_on_run synth_1

reset_run impl_1
launch_runs impl_1 -jobs 4

wait_on_run impl_1
launch_runs impl_1 -to_step write_bitstream -jobs 4
wait_on_run impl_1

open_run impl_1
write_debug_probes   -force ./results/top.ltx
write_hw_platform -include_bit -fixed -force -file ./results/top.xsa
report_timing_summary -file ./results/timing.rpt
report_utilization    -file ./results/utilization.rpt

#set_property BITSTREAM.GENERAL.COMPRESS TRUE [get_designs impl_1]

write_bitstream -verbose -bin_file -force -file ./results/top.bit
file copy -force ./results/top.bit ./proj.runs/impl_1/ 
write_hw_platform -fixed -include_bit -force -file ./results/top.xsa

close_project



