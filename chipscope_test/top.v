
module top(
    input 	wire		clkin100,
    output 	reg[7:0]	led
);
	 
	assign clk = clkin100;
	
	(* keep="true" *)  reg [31:0] count;
	
	always @(posedge clk) begin
		count <= count + 1;
		led <= count[31:24];
	end

endmodule
