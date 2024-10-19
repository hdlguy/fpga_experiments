# AXI register addresses
set VERSION             [expr 4 *0]
set ID                  [expr 4 *1]
set UNUSED2             [expr 4 *2]
set UNUSED3             [expr 4 *3]
set PULSE_PERIOD        [expr 4 *4]
set PULSE_WIDTH         [expr 4 *5]
set PULSE_AMPLITUDE0    [expr 4 *6]
set PULSE_AMPLITUDE1    [expr 4 *7]
set PULSE_AMPLITUDE2    [expr 4 *8]
set PULSE_AMPLITUDE3    [expr 4 *9]
set LED                 [expr 4 *15]


create_hw_axi_txn txn [get_hw_axis hw_axi_1] -address $VERSION -type read -verbose -force
run_hw_axi txn
set rd_data [report_hw_axi_txn txn -w 32 -t x4]  
puts $rd_data;

create_hw_axi_txn txn [get_hw_axis hw_axi_1] -address $ID -type read -verbose -force
run_hw_axi -verbose txn
set rd_data [report_hw_axi_txn txn -w 32 -t x4]  
puts $rd_data;

create_hw_axi_txn txn [get_hw_axis hw_axi_1] -address $PULSE_PERIOD -data [format %8x [expr 2147483648+4096]] -type write -verbose -force
run_hw_axi -verbose txn
create_hw_axi_txn txn [get_hw_axis hw_axi_1] -address $PULSE_PERIOD -type read -verbose -force
run_hw_axi -verbose txn
set rd_data [report_hw_axi_txn txn -w 32 -t x4]  
puts $rd_data;



# flash the LEDs
for {set i 0} {$i < 10} {incr i} {
    #puts "I inside first loop: $i"
    create_hw_axi_txn txn [get_hw_axis hw_axi_1] -address $LED -data [format %8x [expr 2147483648+$i]] -type write -verbose -force
    run_hw_axi -verbose txn
    after 100
}



