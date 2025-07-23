// This module is designed to run on a MicroZed 7020 board.
module top (
    inout logic  [14:0]		DDR_addr,
    inout logic  [2:0]		DDR_ba,
    inout logic  			DDR_cas_n,
    inout logic  			DDR_ck_n,
    inout logic  			DDR_ck_p,
    inout logic  			DDR_cke,
    inout logic  			DDR_cs_n,
    inout logic  [3:0]		DDR_dm,
    inout logic  [31:0]		DDR_dq,
    inout logic  [3:0]		DDR_dqs_n,
    inout logic  [3:0]		DDR_dqs_p,
    inout logic  			DDR_odt,
    inout logic  			DDR_ras_n,
    inout logic  			DDR_reset_n,
    inout logic  			DDR_we_n,
    inout logic  			FIXED_IO_ddr_vrn,
    inout logic  			FIXED_IO_ddr_vrp,
    inout logic  [53:0]		FIXED_IO_mio,
    inout logic  			FIXED_IO_ps_clk,
    inout logic  			FIXED_IO_ps_porb,
    inout logic  			FIXED_IO_ps_srstb
);

    localparam logic[31:0] FPGA_VERSION     = 32'h00000101;
    localparam logic[31:0] FPGA_ID          = 32'hDEADBEEF;
    
    logic           axi_aclk;
    logic [0:0]     axi_aresetn;
    
    logic [11:0]    regfile_addr;
    logic           regfile_clk;
    logic [31:0]    regfile_din;
    logic [31:0]    regfile_dout;
    logic           regfile_en;
    logic           regfile_rst;

    // the Zynq is here just to provide axi_aclk.
    system system_i(
        .DDR_addr(DDR_addr),
        .DDR_ba(DDR_ba),
        .DDR_cas_n(DDR_cas_n),
        .DDR_ck_n(DDR_ck_n),
        .DDR_ck_p(DDR_ck_p),
        .DDR_cke(DDR_cke),
        .DDR_cs_n(DDR_cs_n),
        .DDR_dm(DDR_dm),
        .DDR_dq(DDR_dq),
        .DDR_dqs_n(DDR_dqs_n),
        .DDR_dqs_p(DDR_dqs_p),
        .DDR_odt(DDR_odt),
        .DDR_ras_n(DDR_ras_n),
        .DDR_reset_n(DDR_reset_n),
        .DDR_we_n(DDR_we_n),
        .FIXED_IO_ddr_vrn(FIXED_IO_ddr_vrn),
        .FIXED_IO_ddr_vrp(FIXED_IO_ddr_vrp),
        .FIXED_IO_mio(FIXED_IO_mio),
        .FIXED_IO_ps_clk(FIXED_IO_ps_clk),
        .FIXED_IO_ps_porb(FIXED_IO_ps_porb),
        .FIXED_IO_ps_srstb(FIXED_IO_ps_srstb),
		//    
		.axi_aresetn(axi_aresetn),
		.axi_aclk(axi_aclk),
        //
        .regfile_addr       (regfile_addr),
        .regfile_clk        (regfile_clk),
        .regfile_din        (regfile_din),
        .regfile_dout       (regfile_dout),
        .regfile_en         (regfile_en),
        .regfile_rst        (regfile_rst),
        .regfile_we         (regfile_we)        
    );


    // This register file gives software contol over unit under test (UUT).
    localparam int Nregs = 16;
    logic [Nregs-1:0][31:0] slv_reg, slv_read;

    assign slv_read[0] = FPGA_ID;
    assign slv_read[1] = FPGA_VERSION;

    assign slv_read[2] = slv_reg[2];
    assign slv_read[3] = slv_reg[3];
    assign slv_read[4] = slv_reg[4];
    assign slv_read[5] = slv_reg[5];
    assign slv_read[6] = slv_reg[6];
    assign slv_read[7] = slv_reg[7];
    assign slv_read[8] = slv_reg[8];
    assign slv_read[ 9] = slv_reg[ 9];
    assign slv_read[10] = slv_reg[10];
    assign slv_read[11] = slv_reg[11];
    assign slv_read[12] = slv_reg[12];
    assign slv_read[13] = slv_reg[13];
    assign slv_read[14] = slv_reg[14];
    assign slv_read[15] = slv_reg[15];
    
    mem_regfile #(
       .Naddr(4)  // 16 registers
    ) uut (
        .clk        (regfile_clk),
        .addr       (regfile_addr[5:2]),
        .wr_data    (regfile_din),
        .rd_data    (regfile_dout),
        .en         (regfile_en),
        .reset      (regfile_rst),
        .we         (regfile_we),
        //
        .reg_val    (slv_reg),
        .pul_val    (),
        .read_val   (slv_read)
    );

endmodule

