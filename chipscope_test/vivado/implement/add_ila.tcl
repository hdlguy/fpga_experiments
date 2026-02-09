# this subscript adds an In-system Logic Analyzer (ILA) to the design

#set ila_clock clk_BUFG
set ila_clock clkin100_IBUF_BUFG
#
set_property mark_debug true [ get_nets {count[*]}]
#
create_debug_core top_ila ila
set_property C_DATA_DEPTH   8192 [get_debug_cores top_ila]
set_property C_EN_STRG_QUAL true  [get_debug_cores top_ila]
set_property C_ADV_TRIGGER  true  [get_debug_cores top_ila]
set_property ALL_PROBE_SAME_MU_CNT 4  [get_debug_cores top_ila]
#set_property TRIGGER_COMPARE_VALUE true [get_debug_cores top_ila]
#
# Now find all the nets that are marked for debug.
set ila_nets [get_nets -hier -filter {MARK_DEBUG==1}]
set num_ila_nets [llength [get_nets [list $ila_nets]]]

set_property port_width 1 [get_debug_ports top_ila/clk]
set_property port_width $num_ila_nets [get_debug_ports top_ila/probe0]
connect_debug_port top_ila/probe0 [lsort -dictionary [get_nets [list $ila_nets ]]]
get_nets [list $ila_nets]
connect_debug_port top_ila/clk [get_nets [list $ila_clock ]]

#write_debug_probes -force ./results/top.ltx

