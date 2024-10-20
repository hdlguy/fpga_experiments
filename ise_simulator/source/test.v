
module test (
    input   wire        clkin100,
    input   wire[3:0]   a, b,
    output  reg[7:0]   p
);

    wire clk;
    assign clk = clkin100;

    always @(posedge clk) begin
        p <= a * b;
    end

endmodule

