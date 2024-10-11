
module top(
    input   logic       clkin100,
    inout   logic[3:0]  synclk_p,
    inout   logic[3:0]  synclk_n,
    output  logic[7:0]  led
);

    // make 40MHz clock
    logic clk;
    clkwiz clkwiz_inst (.clk_out1(clk), .clk_in1(clkin100));
    
    logic[3:0] txclk, rxclk;  
    generate for (genvar i=0; i<4; i++) begin  
        ODDR #(.DDR_CLK_EDGE("SAME_EDGE"), .INIT(1'b0), .SRTYPE("SYNC")) ODDR_inst (.Q(txclk[i]), .C(clk), .CE(1'b1), .D1(1'b1), .D2(1'b0), .R(1'b0), .S(1'b0));
        OBUFTDS #(.IOSTANDARD("DEFAULT"), .SLEW("SLOW")) OBUFTDS_inst (.O(synclk_p[i]), .OB(synclk_n[i]), .I(txclk[i]), .T(1'b0));
        IBUFDS #(.DIFF_TERM("TRUE"), .IBUF_LOW_PWR("TRUE"), .IOSTANDARD("DEFAULT")) IBUFDS_inst (.O(rxclk[i]), .I(synclk_p[i]), .IB(synclk_n[i]));
    end endgenerate
    
    // flash an led with the system clock   
    logic[25:0] clk_count=0;
    always_ff @(posedge clk) begin
        clk_count <= clk_count + 1;
        led[7] <= clk_count[24];
    end
    
    // flash some leds with the received clocks
    logic[3:0][27:0] led_count;
    generate for (genvar i=0; i<4; i++) begin  
        always_ff @(posedge rxclk[i]) begin 
            led_count[i] <= led_count[i] + 1;
            led[i] <= led_count[i][24];
        end   
    end endgenerate     

endmodule
