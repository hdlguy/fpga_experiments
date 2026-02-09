`timescale 1ns / 1ps

module datagen_tb ();

    logic       trigger;
    logic [7:0] m_axis_tdata;
    logic       m_axis_tlast;
    logic       m_axis_tready=0;
    logic       m_axis_tvalid;

    localparam time clk_period = 10; logic clk=0; always #(clk_period/2) clk=~clk;

    datagen uut (.*);

    initial begin
    
//        m_axis_tready = 0;
        trigger = 0;
        #(clk_period*100);

//        m_axis_tready = 1;
        #(clk_period*100);

        forever begin
            trigger = 1;
            #(clk_period*1);

            trigger = 0;
            #(clk_period*10000);
        end

    end
    
    always_ff @(posedge clk) m_axis_tready <= ~m_axis_tready;

endmodule

/*
module datagen(
    //
    input clk,
    input trigger,
    //
    output logic [7:0]  m_axis_tdata,
    output logic        m_axis_tlast,
    input  logic        m_axis_tready,
    output logic        m_axis_tvalid
);
*/


