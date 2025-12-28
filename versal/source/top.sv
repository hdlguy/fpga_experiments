
module top(
    output  logic[7:0]  led
);

    logic pl0_ref_clk;
    logic pl0_resetn;

    system system_i( 
        .pl0_ref_clk    (pl0_ref_clk),
        .pl0_resetn     (pl0_resetn)
    );
    
    logic[7:0] led_int = 0;
    always_ff @(posedge pl0_ref_clk) begin
        led_int <= led_int + 1;
        led <= led_int;
    end

endmodule
