
module mem_regfile #(
    parameter int Naddr = 4+2,      // number of byte address bits
    parameter int Nregs = 2**(Naddr-2)
) (
    input   logic                   clk,
    input   logic[Naddr-1;0]        addr,
    input   logic[31:0]             wr_data,
    output  logic[31:0]             rd_data,
    input   logic                   enable,
    input   logic                   reset,
    input   logic[3:0]              wr_enable,
    //
    output  logic[Nregs-1:0][31:0]  reg_val,    // current register value
    output  logic[Nregs-1:0][31:0]  pulse_val,  // single cycle pulse for any bits written as 1. (self-clearing)
    input   logic[Nregs-1:0][31:0]  read_val    // array of values that can be read back.
);


    always_ff @(posedge clk) begin
    end


endmodule

