// trivial design to investigate deleted .tcl files.
module top (
    input   logic       clkin100,
    output  logic[7:0]  led
);

    logic clk;
    assign clk = clkin100;

    // generate the power up reset.
    logic reset;
    logic[15:0] reset_count = -1;
    always_ff @(posedge clk) begin
        if (reset_count != 0) begin
            reset_count <= reset_count - 1;
            reset <= 1;
        end else begin
            reset <= 0;
        end
    end
	
	logic[31:0] led_count=0;
	always_ff @(posedge clk) begin
	   if (reset) begin
	       led_count <= 0;
	       led <= 0;
	   end else begin
	       led_count <= led_count + 1;
	       led <= led_count[31:24];
	   end
	end	
	
	top_ila ila_inst (.clk(clk), .probe0(led_count));

endmodule


