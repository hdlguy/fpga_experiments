// top.sv - experiment with generic to make common source file for two board versions.

module top #(
    parameter int BOARD_VERSION
)(
    // board 0 specific IO
    input   logic           bd0_clk,
    input   logic[7:0]      bd0_inputa, bd0_inputb,
    output  logic[7:0]      bd0_sum,
    // board 1 specific IO
    input   logic           bd1_clk,
    input   logic[7:0]      bd1_inputa, bd1_inputb,
    output  logic[7:0]      bd1_sum,
    // common IO
    input   logic           clk100,
    input   logic[7:0]      mult_a, mult_b,
    output  logic[15:0]     prod
);

    localparam int BOARD0 = 0;
    localparam int BOARD1 = 1;

    // board 0 specific logic
    generate if(BOARD_VERSION == BOARD0) begin
        logic clk0;
        assign clk0 = bd0_clk;
        logic[7:0] bd0_inputa_reg, bd0_inputb_reg;
        always_ff @(posedge clk0) begin
            bd0_inputa_reg <= bd0_inputa;
            bd0_inputb_reg <= bd0_inputb;
            bd0_sum <= bd0_inputa_reg + bd0_inputb_reg;
        end
    end else begin
        assign bd0_sum = 0;
    end endgenerate

    // board 1 specific logic
    generate if(BOARD_VERSION == BOARD1) begin
        logic clk1;
        assign clk1 = bd1_clk;
        logic[7:0] bd1_inputa_reg, bd1_inputb_reg;
        always_ff @(posedge clk1) begin
            bd1_inputa_reg <= bd1_inputa;
            bd1_inputb_reg <= bd1_inputb;
            bd1_sum <= bd1_inputa_reg + bd1_inputb_reg;
        end
    end else begin
        assign bd1_sum = 0;
    end endgenerate

    // common logic
    logic clk;
    assign clk = clk100;

    always_ff @(posedge clk) begin
        prod <= $signed(mult_a) * $signed(mult_b);
    end

endmodule


