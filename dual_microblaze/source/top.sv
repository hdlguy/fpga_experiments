//
module top (
    input   logic       rstn,
    input   logic       clkin100,
    output  logic[7:0]  led,
    input   logic       usb_uart_rxd,
    output  logic       usb_uart_txd
);

    logic clk;
    assign clk = clkin100;

    logic resetn=0;
    always_ff @(posedge clk) resetn <= rstn;
        
    system system_i (
        .resetn(resetn),
        .clkin(clk),
        .usb_uart_rxd(usb_uart_rxd),
        .usb_uart_txd(usb_uart_txd),
        .gpio0_tri_o(led[3:0]),
        .gpio1_tri_o(led[7:4])
    );
    
endmodule


