// THIS FILE IS AUTOGENERATED BY wb_intercon_gen
// ANY MANUAL CHANGES WILL BE LOST
wire [31:0] wb_m2s_master_adr;
wire [31:0] wb_m2s_master_dat;
wire  [3:0] wb_m2s_master_sel;
wire        wb_m2s_master_we;
wire        wb_m2s_master_cyc;
wire        wb_m2s_master_stb;
wire  [2:0] wb_m2s_master_cti;
wire  [1:0] wb_m2s_master_bte;
wire [31:0] wb_s2m_master_dat;
wire        wb_s2m_master_ack;
wire        wb_s2m_master_err;
wire        wb_s2m_master_rty;
wire [31:0] wb_m2s_spi_adr;
wire  [7:0] wb_m2s_spi_dat;
wire  [3:0] wb_m2s_spi_sel;
wire        wb_m2s_spi_we;
wire        wb_m2s_spi_cyc;
wire        wb_m2s_spi_stb;
wire  [2:0] wb_m2s_spi_cti;
wire  [1:0] wb_m2s_spi_bte;
wire  [7:0] wb_s2m_spi_dat;
wire        wb_s2m_spi_ack;
wire        wb_s2m_spi_err;
wire        wb_s2m_spi_rty;

wb_intercon wb_intercon0
   (.wb_clk_i        (wb_clk),
    .wb_rst_i        (wb_rst),
    .wb_master_adr_i (wb_m2s_master_adr),
    .wb_master_dat_i (wb_m2s_master_dat),
    .wb_master_sel_i (wb_m2s_master_sel),
    .wb_master_we_i  (wb_m2s_master_we),
    .wb_master_cyc_i (wb_m2s_master_cyc),
    .wb_master_stb_i (wb_m2s_master_stb),
    .wb_master_cti_i (wb_m2s_master_cti),
    .wb_master_bte_i (wb_m2s_master_bte),
    .wb_master_dat_o (wb_s2m_master_dat),
    .wb_master_ack_o (wb_s2m_master_ack),
    .wb_master_err_o (wb_s2m_master_err),
    .wb_master_rty_o (wb_s2m_master_rty),
    .wb_spi_adr_o    (wb_m2s_spi_adr),
    .wb_spi_dat_o    (wb_m2s_spi_dat),
    .wb_spi_sel_o    (wb_m2s_spi_sel),
    .wb_spi_we_o     (wb_m2s_spi_we),
    .wb_spi_cyc_o    (wb_m2s_spi_cyc),
    .wb_spi_stb_o    (wb_m2s_spi_stb),
    .wb_spi_cti_o    (wb_m2s_spi_cti),
    .wb_spi_bte_o    (wb_m2s_spi_bte),
    .wb_spi_dat_i    (wb_s2m_spi_dat),
    .wb_spi_ack_i    (wb_s2m_spi_ack),
    .wb_spi_err_i    (wb_s2m_spi_err),
    .wb_spi_rty_i    (wb_s2m_spi_rty));

