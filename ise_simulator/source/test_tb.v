
module test_tb ();

    reg[3:0]   a, b;
    wire[7:0]   p;

    localparam clk_period = 10; reg clk = 0; always #(clk_period/2) clk = ~clk;

    test uut (.clkin100(clk), .a(a), .b(b), .p(p));
	 
	 integer i;
	 initial begin
		a = 0;
		b = 0;
		#(clk_period*1);
		for (i=0; i<100; i=i+1) begin
			a = a+13;
			b = b+11;
			#(clk_period*1);
		end
		$stop();
	 end

endmodule

