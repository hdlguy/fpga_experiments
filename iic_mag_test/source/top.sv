// 
module top (
    input   logic       clkin100,
    input   logic       rstn,
    output  logic[7:0]  led,
    input   logic       usb_uart_rxd,
    output  logic       usb_uart_txd,
    inout   logic       iic_scl,
    inout   logic       iic_sda
);

	localparam int Naddr = 6;
    localparam int Nregs = 2**Naddr;
    //localparam logic[Nregs-1:0][31:0] init_reg = 0;
    localparam logic[Nregs-1:0][31:0] init_reg = {
        32'hffff_ffff, 32'heeee_eeee, 32'hdddd_dddd, 32'hcccc_cccc,
        32'hbbbb_bbbb, 32'haaaa_aaaa, 32'h9999_9999, 32'h8888_8888,
        32'h7777_7777, 32'h6666_6666, 32'h5555_5555, 32'h4444_4444,
        32'h3333_3030, 32'h2222_2222, 32'h1111_1111, 32'h0000_0000
    };
	    
    logic [11:0]regfile_addr;
    logic regfile_clk;
    logic [31:0]regfile_din;
    logic [31:0]regfile_dout;
    logic regfile_en;
    logic [3:0]regfile_we;
    
    logic iic_scl_i, iic_scl_o, iic_scl_t, iic_sda_i, iic_sda_o, iic_sda_t;     
    
    logic axi_aclk;       
    
    system system_i (
        .clkin(clkin100),
        .resetn(rstn),
        //
        .axi_aclk       (axi_aclk),
        .axi_aresetn    (),
        //
        .usb_uart_rxd(usb_uart_rxd),
        .usb_uart_txd(usb_uart_txd),
        //
        .iic_scl_i      (iic_scl_i), 
        .iic_scl_o      (iic_scl_o), 
        .iic_scl_t      (iic_scl_t), 
        .iic_sda_i      (iic_sda_i), 
        .iic_sda_o      (iic_sda_o), 
        .iic_sda_t      (iic_sda_t),     
        //
        .regfile_addr   (regfile_addr),
        .regfile_clk    (regfile_clk),
        .regfile_din    (regfile_din),
        .regfile_dout   (regfile_dout),
        .regfile_en     (regfile_en),
        .regfile_rst    (),
        .regfile_we     (regfile_we)        
    );
    
    IOBUF #(.DRIVE(12), .IBUF_LOW_PWR("TRUE"), .IOSTANDARD("DEFAULT"), .SLEW("SLOW")) scl_IOBUF_inst (.O(iic_scl_i), .IO(iic_scl), .I(iic_scl_o), .T(iic_scl_t));
    IOBUF #(.DRIVE(12), .IBUF_LOW_PWR("TRUE"), .IOSTANDARD("DEFAULT"), .SLEW("SLOW")) sda_IOBUF_inst (.O(iic_sda_i), .IO(iic_sda), .I(iic_sda_o), .T(iic_sda_t));
	
	
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
		
	
	iic_ila iic_ila_inst(.clk(axi_aclk), .probe0({iic_scl_i, iic_scl_t}), .probe1({iic_sda_i, iic_sda_t}));  // 2,2

endmodule


