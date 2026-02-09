// trivial design to investigate byte-wide streaming data to and from a Microblaze
module top (
    input   logic       clkin100,
    input   logic       rstn,
    output  logic[7:0]  led,
    input   logic       usb_uart_rxd,
    output  logic       usb_uart_txd
);

    logic clk;
    assign clk = clkin100;
    
    logic [7:0] flash_s_axis_tdata;
    logic       flash_s_axis_tlast;
    logic       flash_s_axis_tready;
    logic       flash_s_axis_tvalid;
    
    logic [7:0] flash_m_axis_tdata;
    logic [0:0] flash_m_axis_tkeep;
    logic       flash_m_axis_tlast;
    logic       flash_m_axis_tready;
    logic       flash_m_axis_tvalid;
    
    system system_i (
        //
        .clkin                  (clk),
        .resetn                 (rstn),
        .usb_uart_rxd           (usb_uart_rxd),
        .usb_uart_txd           (usb_uart_txd),
        //
        .flash_s_axis_tdata     (flash_s_axis_tdata),
        .flash_s_axis_tlast     (flash_s_axis_tlast),
        .flash_s_axis_tready    (flash_s_axis_tready),
        .flash_s_axis_tvalid    (flash_s_axis_tvalid),
        //
        .flash_m_axis_tdata     (flash_m_axis_tdata),
        .flash_m_axis_tkeep     (flash_m_axis_tkeep),
        .flash_m_axis_tlast     (flash_m_axis_tlast),
        .flash_m_axis_tready    (flash_m_axis_tready),
        .flash_m_axis_tvalid    (flash_m_axis_tvalid)
    );
    
    // make 1 pulse per second
    logic[31:0] pps_count=0;
    logic pps=0;
    always_ff @(posedge clk) begin
        if (pps_count == 0) begin
            pps_count <= 100000000-1;
            pps <= 1;
        end else begin
            pps_count <= pps_count - 1;
            pps <= 0;
        end
    end
    
    assign flash_m_axis_tready = 1;
        
    // generate stream data
    datagen datagen_inst (.clk(clk), .trigger(pps), .m_axis_tdata(flash_s_axis_tdata), .m_axis_tlast(flash_s_axis_tlast), .m_axis_tready(flash_s_axis_tready), .m_axis_tvalid(flash_s_axis_tvalid));

    logic[7:0] led_count=0;
	always_ff @(posedge clk) begin
	   if (pps) led_count <= led_count + 1; 
	   led <= led_count; 
   end	            
   	
	// debug	
	top_ila ila_inst (.clk(clk), .probe0({pps, flash_s_axis_tdata, flash_s_axis_tlast, flash_s_axis_tready, flash_s_axis_tvalid, flash_m_axis_tdata, flash_m_axis_tkeep, flash_m_axis_tlast, flash_m_axis_tready, flash_m_axis_tvalid})); //24

endmodule


