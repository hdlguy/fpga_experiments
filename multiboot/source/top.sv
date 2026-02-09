
module top1 (
    input   logic           clkin100,
    output  logic[7:0]      led
);

    assign clk = clkin100;

    logic[31:0] led_count=0;
    always_ff @(posedge clk) begin
        led_count <= led_count + 1;
        led <= led_count[31:24];
    end

endmodule

