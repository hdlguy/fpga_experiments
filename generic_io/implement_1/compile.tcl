# Script to compile the FPGA with zynq processor system all the way to bit file.

close_project -quiet
file delete -force results
file mkdir ./results

open_project proj.xpr

update_compile_order -fileset sources_1

reset_run synth_1
launch_runs synth_1 -jobs 8
wait_on_run synth_1
launch_runs impl_1 -to_step write_bitstream -jobs 8
wait_on_run impl_1

open_run impl_1
report_timing_summary   -file   ./results/timing.rpt
report_utilization      -file   ./results/utilization.rpt
write_debug_probes      -force  ./results/top.ltx

close_project

try {
    set result [exec grep VIO ./results/timing.rpt]
    set result "Timing FAILED!: ${result}"
} on error {e} {
    # typically, pattern not found
    set result "Timing Passed!"
}
puts ${result}

