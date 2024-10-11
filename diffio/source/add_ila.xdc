
create_debug_core ila_0 ila
set_property C_DATA_DEPTH 1024          [get_debug_cores ila_0]
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

create_debug_core ila_1 ila
set_property C_DATA_DEPTH 1024          [get_debug_cores ila_1]
set_property C_TRIGIN_EN false          [get_debug_cores ila_1]
set_property C_TRIGOUT_EN false         [get_debug_cores ila_1]
set_property C_ADV_TRIGGER false        [get_debug_cores ila_1]
set_property C_INPUT_PIPE_STAGES 2      [get_debug_cores ila_1]
set_property C_EN_STRG_QUAL false       [get_debug_cores ila_1]
set_property ALL_PROBE_SAME_MU true     [get_debug_cores ila_1]
set_property ALL_PROBE_SAME_MU_CNT 1    [get_debug_cores ila_1]
set_property port_width 1 [get_debug_ports ila_1/clk]
connect_debug_port ila_1/clk [get_nets rxclk_1_BUFG]
set_property port_width 4 [get_debug_ports ila_1/probe0]
connect_debug_port ila_1/probe0 [get_nets {led_count1*[0] led_count1*[1] led_count1*[2] led_count1*[3]}]

create_debug_core ila_2 ila
set_property C_DATA_DEPTH 1024          [get_debug_cores ila_2]
set_property C_TRIGIN_EN false          [get_debug_cores ila_2]
set_property C_TRIGOUT_EN false         [get_debug_cores ila_2]
set_property C_ADV_TRIGGER false        [get_debug_cores ila_2]
set_property C_INPUT_PIPE_STAGES 2      [get_debug_cores ila_2]
set_property C_EN_STRG_QUAL false       [get_debug_cores ila_2]
set_property ALL_PROBE_SAME_MU true     [get_debug_cores ila_2]
set_property ALL_PROBE_SAME_MU_CNT 1    [get_debug_cores ila_2]
set_property port_width 1 [get_debug_ports ila_2/clk]
connect_debug_port ila_2/clk [get_nets rxclk_2_BUFG]
set_property port_width 4 [get_debug_ports ila_2/probe0]
connect_debug_port ila_2/probe0 [get_nets {led_count2*[0] led_count2*[1] led_count2*[2] led_count2*[3]}]

create_debug_core ila_3 ila
set_property C_DATA_DEPTH 1024          [get_debug_cores ila_3]
set_property C_TRIGIN_EN false          [get_debug_cores ila_3]
set_property C_TRIGOUT_EN false         [get_debug_cores ila_3]
set_property C_ADV_TRIGGER false        [get_debug_cores ila_3]
set_property C_INPUT_PIPE_STAGES 2      [get_debug_cores ila_3]
set_property C_EN_STRG_QUAL false       [get_debug_cores ila_3]
set_property ALL_PROBE_SAME_MU true     [get_debug_cores ila_3]
set_property ALL_PROBE_SAME_MU_CNT 1    [get_debug_cores ila_3]
set_property port_width 1 [get_debug_ports ila_3/clk]
connect_debug_port ila_3/clk [get_nets rxclk_3_BUFG]
set_property port_width 4 [get_debug_ports ila_3/probe0]
connect_debug_port ila_3/probe0 [get_nets {led_count3*[0] led_count3*[1] led_count3*[2] led_count3*[3]}]

create_debug_core ila_4 ila
set_property C_DATA_DEPTH 1024          [get_debug_cores ila_4]
set_property C_TRIGIN_EN false          [get_debug_cores ila_4]
set_property C_TRIGOUT_EN false         [get_debug_cores ila_4]
set_property C_ADV_TRIGGER false        [get_debug_cores ila_4]
set_property C_INPUT_PIPE_STAGES 2      [get_debug_cores ila_4]
set_property C_EN_STRG_QUAL false       [get_debug_cores ila_4]
set_property ALL_PROBE_SAME_MU true     [get_debug_cores ila_4]
set_property ALL_PROBE_SAME_MU_CNT 1    [get_debug_cores ila_4]
set_property port_width 1 [get_debug_ports ila_4/clk]
connect_debug_port ila_4/clk [get_nets clk]
set_property port_width 4 [get_debug_ports ila_4/probe0]
connect_debug_port ila_4/probe0 [get_nets {clk_count*[0] clk_count*[1] clk_count*[2] clk_count*[3]}]




