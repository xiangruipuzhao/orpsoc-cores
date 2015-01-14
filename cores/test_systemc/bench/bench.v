module bench (
	input			wb_clk,
	input			wb_rst,

	input		[31:0]	wb_m2s_master_adr, 
	input		[31:0]	wb_m2s_master_dat,
	input		[3:0]	wb_m2s_master_sel,
	input			wb_m2s_master_we,
	input			wb_m2s_master_cyc,
	input			wb_m2s_master_stb,
	input		[2:0] 	wb_m2s_master_cti,
	input		[1:0]	wb_m2s_master_bte,
	output		[31:0]	wb_s2m_master_dat,
	output			wb_s2m_master_ack,
	output			wb_s2m_master_err,
	output			wb_s2m_master_rty,

	inout		[7:0]	gpio_io
);
////////////////////////////////////////////////////////////////////////
//
// Modules interconnections
//
////////////////////////////////////////////////////////////////////////

`include "wb_intercon.vh"

////////////////////////////////////////////////////////////////////////
//
// GPIO
//
////////////////////////////////////////////////////////////////////////

wire [7:0]	gpio_in;
wire [7:0]	gpio_out;
wire [7:0]	gpio_dir;

// Tristate logic for IO
genvar i;
generate
	for (i = 0; i < 8; i = i+1) begin: gpio_tris
		assign gpio_io[i] = gpio_dir[i] ? gpio_out[i] : 1'bz;
		assign gpio_in[i] = gpio_dir[i] ? gpio_out[i] : gpio_io[i];
	end
endgenerate

gpio gpio (
	// GPIO bus
	.gpio_i		(gpio_in),
	.gpio_o		(gpio_out),
	.gpio_dir_o	(gpio_dir),

	// Wishbone slave interface
	.wb_adr_i	(wb_m2s_gpio_adr[0]),
	.wb_dat_i	(wb_m2s_gpio_dat),
	.wb_we_i	(wb_m2s_gpio_we),
	.wb_cyc_i	(wb_m2s_gpio_cyc),
	.wb_stb_i	(wb_m2s_gpio_stb),
	.wb_cti_i	(wb_m2s_gpio_cti),
	.wb_bte_i	(wb_m2s_gpio_bte),
	.wb_dat_o	(wb_s2m_gpio_dat),
	.wb_ack_o	(wb_s2m_gpio_ack),
	.wb_err_o	(wb_s2m_gpio_err),
	.wb_rty_o	(wb_s2m_gpio_rty),

	.wb_clk		(wb_clk),
	.wb_rst		(wb_rst)
);

endmodule
