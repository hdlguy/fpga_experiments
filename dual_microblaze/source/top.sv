// trivial design to investigate deleted .tcl files.
module top (
    input   logic       clkin100,
    output  logic[7:0]  led,
    input   logic       usb_uart_rxd,
    output  logic       usb_uart_txd
);

    logic clk;
    assign clk = clkin100;
    
    system1 system1_i (.clkin(clkin100), .resetn(1'b1), .usb_uart_rxd(usb_uart_rxd), .usb_uart_txd(usb_uart_txd));    
    system2 system2_i (.clkin(clkin100), .resetn(1'b1));
	
	logic[31:0] led_count=0;
	always_ff @(posedge clk) begin
       led_count <= led_count + 1;
       led <= led_count[31:24];
	end	
	
	top_ila ila_inst (.clk(clk), .probe0(led_count));

endmodule


