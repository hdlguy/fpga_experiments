
module datagen(
    //
    input  logic        clk,
    input  logic        trigger,
    //
    output logic [7:0]  m_axis_tdata,
    output logic        m_axis_tlast,
    input  logic        m_axis_tready,
    output logic        m_axis_tvalid
);

    localparam int Ndata = 1024;
    localparam logic[3:0][7:0] control = {8'h40, 8'h00, 8'h00, 8'h03};
  
    logic[15:0] data_count=0;
    logic data_count_clear, control_enable;
    logic[3:0] state=0, next_state;
    always_comb begin
    
        // defaults
        next_state = state;
        data_count_clear = 0;
        m_axis_tvalid = 0;
        control_enable = 0;
        
        case (state)
            
            0: begin
                next_state = 1;
            end
            
            1: begin
                data_count_clear = 1;
                if (trigger) begin
                    next_state = 2;
                end
            end
            
            // put out header
            2: begin
                m_axis_tvalid = 1;
                control_enable = 1;
                if ((m_axis_tvalid) && (m_axis_tready) && (data_count==3)) begin
                    next_state = 3;                   
                end
            end
            
            3: begin
                data_count_clear = 1;
                next_state = 4;    
            end
            
            // put out data
            4: begin           
                m_axis_tvalid = 1;     
                if ((m_axis_tvalid) && (m_axis_tready) && (data_count==Ndata-1)) begin
                    next_state = 5;                   
                end
            end
            
            5: begin
                next_state = 0;
            end
            
            default: begin
                next_state = 0;
            end
            
        endcase
        
    end    
    
    always_ff @(posedge clk) state <= next_state;
    
    always_ff @(posedge clk) begin
        if (data_count_clear) begin
            data_count <= 0;
        end else begin
            if ((m_axis_tready) && (m_axis_tvalid)) begin
                data_count <= data_count + 1;
            end
        end
    end
    
    logic[7:0] pulse_count=0;
    always_ff @(posedge clk) begin
        if (trigger) pulse_count <= pulse_count + 1;
    end
    
    assign m_axis_tlast = (data_count == Ndata-1);
    always_comb begin
        if (control_enable) begin
            m_axis_tdata = control[data_count]; 
        end else begin
            m_axis_tdata = data_count[7:0] + pulse_count;
        end
    end

endmodule

