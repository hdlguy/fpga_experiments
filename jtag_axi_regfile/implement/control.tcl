# AXI register addresses
set VERSION             [expr 4 *0]
set ID                  [expr 4 *1]
set LED                 [expr 4 *3]
puts "**** LED = 0x[format %x $LED]"

create_hw_axi_txn txn [get_hw_axis hw_axi_1] -address [format %x $VERSION] -type read -verbose -force
run_hw_axi txn
set rd_data [report_hw_axi_txn txn -w 32 -t x4]  
puts $rd_data;

create_hw_axi_txn txn [get_hw_axis hw_axi_1] -address [format %x $ID] -type read -verbose -force
run_hw_axi -verbose txn
set rd_data [report_hw_axi_txn txn -w 32 -t x4]  
puts $rd_data;


# flash the LEDs
#for {set i 0} {1} {incr i} {
for {set i 0} {$i < 10} {incr i} {
    create_hw_axi_txn txn [get_hw_axis hw_axi_1] -address [format %x $LED] -data [format %8x [expr 2147483648+$i+5]] -type write -force
    run_hw_axi txn
    after 100
}



