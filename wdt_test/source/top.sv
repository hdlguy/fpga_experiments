// trivial design to investigate deleted .tcl files.
module top (
    input   logic       clkin100,
    input   logic       resetn,
    output  logic[7:0]  led,
    input   logic[3:0]  btn,
    input   logic       usb_uart_rxd,
    output  logic       usb_uart_txd
);

    logic axi_aclk, axi_aresetn, clk;
    assign clk = axi_aclk;
    
    logic [7:0]gpio0_tri_o;
    logic [3:0]gpio1_tri_i;
    
    system system_i (
        .clkin(clkin100),
        .resetn(resetn),
        .axi_aclk(axi_aclk),
        .axi_aresetn(axi_aresetn),
        .gpio0_tri_o(gpio0_tri_o),
        .gpio1_tri_i(gpio1_tri_i),
        .usb_uart_rxd(usb_uart_rxd),
        .usb_uart_txd(usb_uart_txd)
    );
    
    assign led = gpio0_tri_o;
    assign gpio1_tri_i = btn;
    
endmodule
	
//	logic[31:0] led_count=0;
//	always_ff @(posedge clk) begin
//	   if (axi_aresetn == 0) begin
//	       led_count <= 0;
//	       led <= 0;
//	   end else begin
//	       led_count <= led_count + 1;
//	       led <= led_count[31:24];
//	   end
//	end	
	
//	top_ila ila_inst (.clk(clk), .probe0(led_count));

//endmodule


