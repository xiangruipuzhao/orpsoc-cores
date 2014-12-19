#include <systemc.h>
#include <stdint.h>
#include <pthread.h>
#include "/home/franck/openrisc/work/fusesoc/build/test_systemc/sim-verilator/obj_dir/Vgpio.h"
#include "wb_master.h"

Vgpio *DUT  = new Vgpio("Vgpio");
wb_master *MASTER = new wb_master("wb_master");

void *test_thread(void *arg)
{
	sleep(1);
	MASTER->drive_e1();
	pthread_exit(0);
}

int sc_main(int argc, char* argv[])
{
	pthread_t th1;
	void *ret;

	sc_signal	< bool >	rst;
	sc_signal	< uint32_t >	wb_adr;
	sc_signal	< uint32_t >	wb_dat_i;
	sc_signal	< bool >	wb_we;
	sc_signal	< bool >	wb_cyc;
	sc_signal	< bool >	wb_stb;
	sc_signal	< uint32_t >	wb_cti;
	sc_signal	< uint32_t >	wb_bte;
	sc_signal	< uint32_t >	wb_dat_o;
	sc_signal	< bool >	wb_ack;
	sc_signal	< bool >	wb_err;
	sc_signal	< bool >	wb_rty;
	sc_signal	< uint32_t >	gpio_i;
	sc_signal	< uint32_t >	gpio_o;
	sc_signal	< uint32_t >	gpio_dir;

	if (pthread_create (&th1, NULL, test_thread, NULL) < 0) {
		printf("pthread_create error for thread 1\n");
		exit (1);
	}

	sc_clock clk("clk", 10, SC_NS, 0.5);   // Create a clock signal

	MASTER->clk(clk);
	MASTER->rst(rst);

	MASTER->wb_adr(wb_adr);
	MASTER->wb_dat_i(wb_dat_o);
	MASTER->wb_we(wb_we);
	MASTER->wb_cyc(wb_cyc);
	MASTER->wb_stb(wb_stb);
	MASTER->wb_cti(wb_cti);
	MASTER->wb_bte(wb_bte);
	MASTER->wb_dat_o(wb_dat_i);
	MASTER->wb_ack(wb_ack);
	MASTER->wb_err(wb_err);
	MASTER->wb_rty(wb_rty);

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

	sc_start(1000, SC_MS);                // Run simulation

	sc_close_vcd_trace_file(fp);        // close(fp)

	(void)pthread_join (th1, &ret);

	return 0;                           // Return no errors
}
