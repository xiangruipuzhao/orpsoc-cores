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

	output			sck,
	output			mosi,
	input			miso,
	output			ss
);
////////////////////////////////////////////////////////////////////////
//
// Modules interconnections
//
////////////////////////////////////////////////////////////////////////

`include "wb_intercon.vh"

////////////////////////////////////////////////////////////////////////
//
// SIMPLE SPI
//
////////////////////////////////////////////////////////////////////////

simple_spi simple_spi
(
	.clk_i		(wb_clk),
	.rst_i		(wb_rst),
	.cyc_i		(wb_m2s_spi_cyc),
	.stb_i		(wb_m2s_spi_stb),
	.adr_i		(wb_m2s_spi_adr),
	.we_i		(wb_m2s_spi_we),
	.dat_i		(wb_m2s_spi_dat),
	.dat_o		(wb_s2m_spi_dat),
	.ack_o		(wb_s2m_spi_ack),
	.inta_o		(),

	.sck_o		(sck),
	.ss_o		(ss),
	.mosi_o		(mosi),
	.miso_i		(miso)
);

endmodule
