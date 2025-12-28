// trivial design to investigate using to the ICAP to
// reboot the fpga from configuration flash. The 
// AXI ICAP is in the system IPI block diagram.
module top (
    input   logic       clkin100,
    output  logic[7:0]  led,
    input   logic       usb_uart_rxd,
    output  logic       usb_uart_txd
);

    logic axi_aclk, axi_aresetn, clk;
    assign clk = axi_aclk;
    
    system system_i (
        .clkin(clkin100),
        .resetn(1'b1),
        .axi_aclk(axi_aclk),
        .axi_aresetn(axi_aresetn),
        .usb_uart_rxd(usb_uart_rxd),
        .usb_uart_txd(usb_uart_txd)
    );
	
	logic[31:0] led_count=0;
	always_ff @(posedge clk) begin
	   if (axi_aresetn == 0) begin
	       led_count <= 0;
	       led <= 0;
	   end else begin
	       led_count <= led_count + 1;
	       led <= led_count[31:24];
	   end
	end	
	
	top_ila ila_inst (.clk(clk), .probe0(led_count));

endmodule


