// This design is used to test run-time access to the qspi configuration flash.
// The startup block is enabled in the AXI QSPI interface so that SCK is not specifically brought out.
// Also, the JTAG uart is enabled in the debug module so that the usb-uart is not consumed.
//
// QSPI access is implemented in C code derived from Xilinx example code.
//
module top (
    input   logic       clkin100,
    output  logic[7:0]  led,
    inout   logic       qspi_io_io[3:0],
    inout   logic       qspi_ss_io
);

    logic clk;

    // generate the power up reset.
    logic reset;
    logic[7:0] reset_count = 255;
    always_ff @(posedge clk) begin
        if (reset_count != 0) begin
            reset_count <= reset_count - 1;
            reset <= 1;
        end else begin
            reset <= 0;
        end
    end
    assign resetn = ~reset;

    logic           axi_aresetn;
    logic           axi_aclk;
    logic [31:0]    m_axi_araddr;
    logic [2:0]     m_axi_arprot;
    logic           m_axi_arready;
    logic           m_axi_arvalid;
    logic [31:0]    m_axi_awaddr;
    logic [2:0]     m_axi_awprot;
    logic           m_axi_awready;
    logic           m_axi_awvalid;
    logic           m_axi_bready;
    logic [1:0]     m_axi_bresp;
    logic           m_axi_bvalid;
    logic [31:0]    m_axi_rdata;
    logic           m_axi_rready;
    logic [1:0]     m_axi_rresp;
    logic           m_axi_rvalid;
    logic [31:0]    m_axi_wdata;
    logic           m_axi_wready;
    logic [3:0]     m_axi_wstrb;
    logic           m_axi_wvalid;

    logic[3:0] qspi_io_i;
    logic[3:0] qspi_io_o;
    logic[3:0] qspi_io_t;
    logic qspi_sck_i;
    logic qspi_sck_o;
    logic qspi_sck_t;
    logic qspi_ss_i;
    logic qspi_ss_o;    
    logic qspi_ss_t;
    
    logic startup_cfgclk;
    logic startup_cfgmclk;
    logic startup_eos;
    logic startup_preq;

    system system_i (
        .resetn         (resetn),
        .clkin          (clkin100),
        //
        .axi_aclk       (axi_aclk),
        .axi_aresetn    (axi_aresetn),
        .m_axi_araddr   (m_axi_araddr),
        .m_axi_arprot   (m_axi_arprot),
        .m_axi_arready  (m_axi_arready),
        .m_axi_arvalid  (m_axi_arvalid),
        .m_axi_awaddr   (m_axi_awaddr),
        .m_axi_awprot   (m_axi_awprot),
        .m_axi_awready  (m_axi_awready),
        .m_axi_awvalid  (m_axi_awvalid),
        .m_axi_bready   (m_axi_bready),
        .m_axi_bresp    (m_axi_bresp),
        .m_axi_bvalid   (m_axi_bvalid),
        .m_axi_rdata    (m_axi_rdata),
        .m_axi_rready   (m_axi_rready),
        .m_axi_rresp    (m_axi_rresp),
        .m_axi_rvalid   (m_axi_rvalid),
        .m_axi_wdata    (m_axi_wdata),
        .m_axi_wready   (m_axi_wready),
        .m_axi_wstrb    (m_axi_wstrb),
        .m_axi_wvalid   (m_axi_wvalid),
        //
        .qspi_io0_i     (qspi_io_i[0]),
        .qspi_io0_o     (qspi_io_o[0]),
        .qspi_io0_t     (qspi_io_t[0]),
        .qspi_io1_i     (qspi_io_i[1]),
        .qspi_io1_o     (qspi_io_o[1]),
        .qspi_io1_t     (qspi_io_t[1]),
        .qspi_io2_i     (qspi_io_i[2]),
        .qspi_io2_o     (qspi_io_o[2]),
        .qspi_io2_t     (qspi_io_t[2]),
        .qspi_io3_i     (qspi_io_i[3]),
        .qspi_io3_o     (qspi_io_o[3]),
        .qspi_io3_t     (qspi_io_t[3]),
        .qspi_ss_i      (qspi_ss_i),
        .qspi_ss_o      (qspi_ss_o),
        .qspi_ss_t      (qspi_ss_t),
        //
        .startup_cfgclk (startup_cfgclk),
        .startup_cfgmclk(startup_cfgmclk),
        .startup_eos    (startup_eos),
        .startup_preq   (startup_preq)                     
    );
    
    IOBUF qspi_io0_iobuf (.I(qspi_io_o[0]), .IO(qspi_io_io[0]), .O(qspi_io_i[0]), .T(qspi_io_t[0]));
    IOBUF qspi_io1_iobuf (.I(qspi_io_o[1]), .IO(qspi_io_io[1]), .O(qspi_io_i[1]), .T(qspi_io_t[1]));
    IOBUF qspi_io2_iobuf (.I(qspi_io_o[2]), .IO(qspi_io_io[2]), .O(qspi_io_i[2]), .T(qspi_io_t[2]));
    IOBUF qspi_io3_iobuf (.I(qspi_io_o[3]), .IO(qspi_io_io[3]), .O(qspi_io_i[3]), .T(qspi_io_t[3])); 
    IOBUF qspi_ss_iobuf  (.I(qspi_ss_o),    .IO(qspi_ss_io),    .O(qspi_ss_i),    .T(qspi_ss_t));    
    
    localparam logic[31:0] VERSION = 32'h00_01_02_03;
    localparam logic[31:0] ID      = 32'hDEADBEEF;
    localparam integer LOG2_NREGS = 4;
    localparam integer NREGS = 2**LOG2_NREGS;
    logic [NREGS-1:0][31:0] slv_reg, slv_read, slv_wr_pulse;
    
    // connect signals to register file
    assign slv_read[0] = VERSION;
    assign slv_read[1] = ID;
    assign led[3:0] = slv_reg[3][3:0];
    assign slv_read[NREGS-1:2] = slv_reg[NREGS-1:2];

	axi_regfile_v1_0_S00_AXI #	(
		.C_S_AXI_DATA_WIDTH (32),
		.C_S_AXI_ADDR_WIDTH (LOG2_NREGS+2)
	) axi_regfile_inst (
        // register interface
        .slv_read       (slv_read), 
        .slv_reg        (slv_reg),  
        .slv_wr_pulse   (slv_wr_pulse),  
        // axi interface
		.S_AXI_ACLK    (axi_aclk),
		.S_AXI_ARESETN (axi_aresetn),
		.S_AXI_ARADDR  (m_axi_araddr ),
		.S_AXI_ARPROT  (m_axi_arprot ),
		.S_AXI_ARREADY (m_axi_arready),
		.S_AXI_ARVALID (m_axi_arvalid),
		.S_AXI_AWADDR  (m_axi_awaddr ),
		.S_AXI_AWPROT  (m_axi_awprot ),
		.S_AXI_AWREADY (m_axi_awready),
		.S_AXI_AWVALID (m_axi_awvalid),
		.S_AXI_BREADY  (m_axi_bready ),
		.S_AXI_BRESP   (m_axi_bresp  ),
		.S_AXI_BVALID  (m_axi_bvalid ),
		.S_AXI_RDATA   (m_axi_rdata  ),
		.S_AXI_RREADY  (m_axi_rready ),
		.S_AXI_RRESP   (m_axi_rresp  ),
		.S_AXI_RVALID  (m_axi_rvalid ),
		.S_AXI_WDATA   (m_axi_wdata  ),
		.S_AXI_WREADY  (m_axi_wready ),
		.S_AXI_WSTRB   (m_axi_wstrb  ),
		.S_AXI_WVALID  (m_axi_wvalid )
	);
	
	logic[31:0] led_count=0;
	always_ff @(posedge axi_aclk) begin
	   led_count <= led_count + 1;
	   led[7:4] <= led_count[27:24];
	end
	
	//top_ila ila_inst (.clk(axi_aclk), .probe0({axi_aresetn, m_axi_arvalid, m_axi_araddr, m_axi_awvalid, m_axi_awaddr, m_axi_rvalid, m_axi_rdata, m_axi_wvalid, m_axi_wdata})); // 4*32+5=133
	top_ila ila_inst (.clk(axi_aclk), .probe0({qspi_io_i, qspi_io_o, qspi_io_t, qspi_ss_i, qspi_ss_o, qspi_ss_t, startup_cfgclk})); // 16

endmodule


