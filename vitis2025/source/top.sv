// 
module top (
    input   logic       clkin100,
    output  logic[7:0]  led,
    input   logic       usb_uart_rxd,
    output  logic       usb_uart_txd
);

	localparam int Naddr = 4;
    localparam int Nregs = 2**Naddr;
    localparam logic[Nregs-1:0][31:0] init_reg = {
        32'hffff_ffff, 32'heeee_eeee, 32'hdddd_dddd, 32'hcccc_cccc,
        32'hbbbb_bbbb, 32'haaaa_aaaa, 32'h9999_9999, 32'h8888_8888,
        32'h7777_7777, 32'h6666_6666, 32'h5555_5555, 32'h4444_4444,
        32'h3333_3030, 32'h2222_2222, 32'h1111_1111, 32'h0000_0000
    };
	
    logic axi_aclk, axi_aresetn, clk;
    assign clk = axi_aclk;
    
    logic [11:0]regfile_addr;
    logic regfile_clk;
    logic [31:0]regfile_din;
    logic [31:0]regfile_dout;
    logic regfile_en;
    logic regfile_rst;
    logic [3:0]regfile_we;
    
    system system_i (
        .clkin(clkin100),
        .resetn(1'b1),
        //
        .axi_aclk(axi_aclk),
        .axi_aresetn(axi_aresetn),
        //
        .usb_uart_rxd(usb_uart_rxd),
        .usb_uart_txd(usb_uart_txd),
        //
        .regfile_addr   (regfile_addr),
        .regfile_clk    (regfile_clk),
        .regfile_din    (regfile_din),
        .regfile_dout   (regfile_dout),
        .regfile_en     (regfile_en),
        .regfile_rst    (regfile_rst),
        .regfile_we     (regfile_we)        
    );
	
	logic[31:0] led_count=0;
	always_ff @(posedge clk) begin
	   if (axi_aresetn == 0) begin
	       led_count <= 0;
	   end else begin
	       led_count <= led_count + 1;
	   end
	   //led <= led_count[31:24];
	end	
	
	
    logic[Nregs-1:0][31:0]  reg_val, pul_val, read_val;
	mem_regfile #(
	   .Naddr(Naddr),
	   .init_reg(init_reg)
	) uut (
	   .clk(regfile_clk),
	   .addr(regfile_addr[5:2]),
	   .wr_data(regfile_din),
	   .rd_data(regfile_dout),
	   .en(regfile_en),
	   .reset(regfile_rst),
	   .we(regfile_we),
	   //
	   .reg_val(reg_val),
	   .pul_val(pul_val),
	   .read_val(read_val)
	);
	
	assign read_val[0] = 32'hdeadbeef;
	assign read_val[1] = 32'h01234567;
    assign led = reg_val[2][7:0];
	assign read_val[Nregs-1:2] = reg_val[Nregs-1:2];
		
	
	top_ila ila_inst (.clk(clk), .probe0({regfile_addr, regfile_din, regfile_dout, regfile_en, regfile_rst, regfile_we, pul_val[3]})); // 114

endmodule


