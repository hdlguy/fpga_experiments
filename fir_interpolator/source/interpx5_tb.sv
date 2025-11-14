`timescale 1ns / 1ps

module interpx5_tb();

    logic aclk;                       
    logic s_axis_data_tvalid;         
    logic s_axis_data_tready;        
    logic [15 : 0] s_axis_data_tdata; 
    logic m_axis_data_tvalid;        
    logic [23 : 0] m_axis_data_tdata; 
    
    localparam time clk_period=10; logic clk=0; always #(clk_period/2) clk=~clk;
    assign aclk=clk;
    

    interpx5 fir_real (
        .aclk               (aclk),                            
        .s_axis_data_tvalid (s_axis_data_tvalid),
        .s_axis_data_tready (s_axis_data_tready),
        .s_axis_data_tdata  (s_axis_data_tdata),  
        .m_axis_data_tvalid (m_axis_data_tvalid),
        .m_axis_data_tdata  (m_axis_data_tdata)   
    );  
    
    localparam real pi = $acos(-1);
    localparam N=2000;
    real phase=0.0, freq=-pi, cr=2.0*pi/N;
    
    logic[15:0] din_real;
    always_ff @(posedge clk) begin
        if (s_axis_data_tready) begin
            din_real = $signed($rtoi(32767.0*$cos(phase)));
            phase <= phase + freq;
            freq <= freq + cr;
        end    
    end
    
    assign s_axis_data_tvalid=1;
    assign s_axis_data_tdata = din_real;
    
    // saturation
    logic[15:0] dout_real;
    always_comb begin
        if (m_axis_data_tdata[16] == m_axis_data_tdata[15]) begin
            dout_real = m_axis_data_tdata[15:0];
        end else begin
            if (m_axis_data_tdata[16] == 1'b1) begin
                dout_real = -32767;
            end else begin
                dout_real = +32767;
            end
        end
    end

endmodule