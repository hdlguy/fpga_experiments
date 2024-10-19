#
open_run synth_1

create_debug_core ila_0 ila
set_property C_DATA_DEPTH 1024         [get_debug_cores ila_0]
set_property C_TRIGIN_EN false          [get_debug_cores ila_0]
set_property C_TRIGOUT_EN false         [get_debug_cores ila_0]
set_property C_ADV_TRIGGER false        [get_debug_cores ila_0]
set_property C_INPUT_PIPE_STAGES 2      [get_debug_cores ila_0]
set_property C_EN_STRG_QUAL false       [get_debug_cores ila_0]
set_property ALL_PROBE_SAME_MU true     [get_debug_cores ila_0]
set_property ALL_PROBE_SAME_MU_CNT 1    [get_debug_cores ila_0]

set_property port_width 1 [get_debug_ports ila_0/clk]
connect_debug_port ila_0/clk [get_nets rxclk_0_BUFG]
set_property port_width 4 [get_debug_ports ila_0/probe0]
connect_debug_port ila_0/probe0 [get_nets {led_count0*[0] led_count0*[1] led_count0*[2] led_count0*[3]}]

#led_count0_reg_n_0_[0]
