// trivial design to investigate deleted .tcl files.
module top (
    input   logic       clkin100,
    output  logic[7:0]  led,
    input   logic       usb_uart_rxd,
    output  logic       usb_uart_txd
);

    logic clk;
    assign clk = clkin100;
    
    logic debug_capture;
    logic debug_clk;
    logic debug_disable;
    logic[0:7] debug_reg_en;
    logic debug_rst;
    logic debug_shift;
    logic debug_tdi;
    logic debug_tdo;
    logic debug_update;
    

    system1 system1_i (
        .clkin(clkin100), 
        .resetn(1'b1), 
        .usb_uart_rxd(usb_uart_rxd), 
        .usb_uart_txd(usb_uart_txd),
        .gpio_tri_o(led[3:0]),
        //
        .debug_capture(debug_capture),
        .debug_clk(debug_clk),
        .debug_disable(debug_disable),
        .debug_reg_en(debug_reg_en),
        .debug_rst(debug_rst),
        .debug_shift(debug_shift),
        .debug_tdi(debug_tdi),
        .debug_tdo(debug_tdo),
        .debug_update(debug_update)
    );   

     
    system2 system2_i (
        .clkin(clkin100), 
        .resetn(1'b1),        
        .gpio_tri_o(led[7:4]),
        //
        .debug_capture(debug_capture),
        .debug_clk(debug_clk),
        .debug_disable(debug_disable),
        .debug_reg_en(debug_reg_en),
        .debug_rst(debug_rst),
        .debug_shift(debug_shift),
        .debug_tdi(debug_tdi),
        .debug_tdo(debug_tdo),
        .debug_update(debug_update)
    );
	
	//top_ila ila_inst (.clk(clk), .probe0(led_count));

endmodule


