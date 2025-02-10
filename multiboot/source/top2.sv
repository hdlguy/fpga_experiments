
module top2 (
    input   logic           clkin100,
    output  logic[7:0]      led
);

    assign clk = clkin100;

    logic[31:0] led_count=0;
    always_ff @(posedge clk) begin
        led_count <= led_count + 1;
        led <= led_count[27:20];
    end

endmodule

