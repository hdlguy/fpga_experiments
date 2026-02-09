// 
module top (
    //
    inout  logic [63:0] DDR4_dq,
    inout  logic [7:0]  DDR4_dqs_t,
    inout  logic [7:0]  DDR4_dqs_c,
    output logic [16:0] DDR4_adr,
    output logic [1:0]  DDR4_ba,
    output logic [0:0]  DDR4_bg,
    output logic [0:0]  DDR4_act_n,
    output logic [0:0]  DDR4_reset_n,
    output logic [0:0]  DDR4_ck_t,
    output logic [0:0]  DDR4_ck_c,
    output logic [0:0]  DDR4_cke,
    output logic [0:0]  DDR4_cs_n,
    inout  logic [7:0]  DDR4_dm_n,
    output logic [0:0]  DDR4_odt,
    //
    input  logic [0:0]  sys_clk_n,
    input  logic [0:0]  sys_clk_p
);

	localparam int Naddr = 4;
    localparam int Nregs = 2**Naddr;
    //localparam logic[Nregs-1:0][31:0] init_reg = 0;
    localparam logic[Nregs-1:0][31:0] init_reg = {
        32'hffff_ffff, 32'heeee_eeee, 32'hdddd_dddd, 32'hcccc_cccc,
        32'hbbbb_bbbb, 32'haaaa_aaaa, 32'h9999_9999, 32'h8888_8888,
        32'h7777_7777, 32'h6666_6666, 32'h5555_5555, 32'h4444_4444,
        32'h3333_3030, 32'h2222_2222, 32'h1111_1111, 32'h0000_0000
    };
	    
    logic [15:0]regfile_addr;
    logic regfile_clk;
    logic [31:0]regfile_din;
    logic [31:0]regfile_dout;
    logic regfile_en;
    logic [3:0]regfile_we;
    
    system system_inst (
        .DDR4_dq(DDR4_dq),          // inout  wire [63:0] DDR4_dq
        .DDR4_dqs_t(DDR4_dqs_t),    // inout  wire [7:0] DDR4_dqs_t
        .DDR4_dqs_c(DDR4_dqs_c),    // inout  wire [7:0] DDR4_dqs_c
        .DDR4_adr(DDR4_adr),        // output wire [16:0] DDR4_adr
        .DDR4_ba(DDR4_ba),          // output wire [1:0] DDR4_ba
        .DDR4_bg(DDR4_bg),          // output wire [0:0] DDR4_bg
        .DDR4_act_n(DDR4_act_n),    // output wire [0:0] DDR4_act_n
        .DDR4_reset_n(DDR4_reset_n),// output wire [0:0] DDR4_reset_n
        .DDR4_ck_t(DDR4_ck_t),      // output wire [0:0] DDR4_ck_t
        .DDR4_ck_c(DDR4_ck_c),      // output wire [0:0] DDR4_ck_c
        .DDR4_cke(DDR4_cke),        // output wire [0:0] DDR4_cke
        .DDR4_cs_n(DDR4_cs_n),      // output wire [0:0] DDR4_cs_n
        .DDR4_dm_n(DDR4_dm_n),      // inout  wire [7:0] DDR4_dm_n
        .DDR4_odt(DDR4_odt),        // output wire [0:0] DDR4_odt
        //
        .sys_clk_n(sys_clk_n),      // input  wire [0:0] sys_clk_n
        .sys_clk_p(sys_clk_p),      // input  wire [0:0] sys_clk_p
        //
        .regfile_addr(regfile_addr), // output wire [12:0] regfile_addr
        .regfile_clk(regfile_clk), // output wire regfile_clk
        .regfile_din(regfile_din), // output wire [31:0] regfile_din
        .regfile_dout(regfile_dout), // input wire [31:0] regfile_dout
        .regfile_en(regfile_en), // output wire regfile_en
        .regfile_rst(regfile_rst), // output wire regfile_rst
        .regfile_we(regfile_we) // output wire [3:0] regfile_we
    );
	
	
    logic[Nregs-1:0][31:0]  reg_val, pul_val, read_val;
	mem_regfile #(
	   .Naddr       (Naddr),
	   .init_reg    (init_reg)
	) uut (
	   .clk         (regfile_clk),
	   .addr        (regfile_addr[Naddr+2-1:2]),
	   .wr_data     (regfile_din),
	   .rd_data     (regfile_dout),
	   .en          (regfile_en),
	   .we          (regfile_we),
	   //
	   .reg_val     (reg_val),
	   .pul_val     (pul_val),
	   .read_val    (read_val)
	);
	
	assign read_val[0] = 32'hdeadbeef;
	assign read_val[1] = 32'h01234567;
	assign read_val[Nregs-1:2] = reg_val[Nregs-1:2];

    assign led = reg_val[2][7:0];
		
	
	//top_ila ila_inst (.clk(regfile_clk), .probe0({regfile_addr, regfile_din, regfile_dout, regfile_en, 1'b0, regfile_we, pul_val[3]})); // 114

endmodule


