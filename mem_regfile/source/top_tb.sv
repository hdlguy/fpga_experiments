`timescale 1ns / 1ps

module top_tb();

    logic[7:0]  led;
    logic       usb_uart_rxd;
    logic       usb_uart_txd;
    
    localparam time clk_period=10; logic clkin100=0; always #(clk_period/2) clkin100=~clkin100;

    top uut(.*);

endmodule



