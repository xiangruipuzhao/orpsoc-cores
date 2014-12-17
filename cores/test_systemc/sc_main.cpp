//------------------------------------------------------------------
// Simple Testbench for dff flipflop file
//
// SystemC for VHDL engineers
// (c)www.ht-lab.com
//------------------------------------------------------------------

#include <systemc.h>
#include "Vgpio.h"

int sc_main(int argc, char* argv[])
{
	sc_signal < bool > rst;
	sc_signal < bool > wb_adr;
	sc_signal < uint32_t > wb_dat_i;
	sc_signal < bool > wb_we;
	sc_signal < bool > wb_cyc;
	sc_signal < bool > wb_stb;
	sc_signal < uint32_t > wb_cti;
	sc_signal < uint32_t > wb_bte;
	sc_signal < uint32_t > wb_dat_o;
	sc_signal < bool > wb_ack;
	sc_signal < bool > wb_err;
	sc_signal < bool > wb_rty;
	sc_signal < uint32_t > gpio_i;
	sc_signal < uint32_t > gpio_o;
	sc_signal < uint32_t > gpio_dir;

	sc_clock clk("clk", 10, SC_NS, 0.5);   // Create a clock signal

	Vgpio *DUT  = new Vgpio("Vgpio");

	DUT->wb_clk(clk);
	DUT->wb_rst(rst);

	DUT->wb_adr_i(wb_adr);
	DUT->wb_dat_i(wb_dat_i);
	DUT->wb_we_i(wb_we);
	DUT->wb_cyc_i(wb_cyc);
	DUT->wb_stb_i(wb_stb);
	DUT->wb_cti_i(wb_cti);
	DUT->wb_bte_i(wb_bte);
	DUT->wb_dat_o(wb_dat_o);
	DUT->wb_ack_o(wb_ack);
	DUT->wb_err_o(wb_err);
	DUT->wb_rty_o(wb_rty);

	DUT->gpio_i(gpio_i);
	DUT->gpio_o(gpio_o);
	DUT->gpio_dir_o(gpio_dir);

	sc_trace_file *fp;                  // Create VCD file
	fp=sc_create_vcd_trace_file("wave");
	fp->set_time_unit(100, SC_PS);      // set tracing resolution to ns
	sc_trace(fp, clk, "clk");             // Add signals to trace file
	sc_trace(fp, rst, "rst");             // Add signals to trace file
	sc_trace(fp, wb_adr, "wb_adr");             // Add signals to trace file
	sc_trace(fp, wb_cyc, "wb_cyc");             // Add signals to trace file
	sc_trace(fp, wb_stb, "wb_stb");             // Add signals to trace file
	sc_trace(fp, wb_dat_i, "wb_dat_i");             // Add signals to trace file

	rst = 1;
	sc_start(31, SC_NS);                // Run simulation
	rst = 0;
	sc_start(200, SC_NS);                // Run simulation

	wb_adr = 1;
	wb_cyc = 1;
	wb_stb = 1;
	wb_dat_i = 0xff;
	sc_start(10, SC_NS);                // Run simulation

	wb_cyc = 0;
	wb_stb = 0;
	sc_start(10, SC_NS);                // Run simulation

	sc_close_vcd_trace_file(fp);        // close(fp)

	return 0;                           // Return no errors
}
