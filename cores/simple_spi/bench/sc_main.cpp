#include <systemc.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <Vbench.h>
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "wb_master.h"
#include "wb_xfer.h"
#include "or1ksim.h"
#include "generic.h"

#define OR1KSIM_CONF_FILE	"/sw/or1ksim.cfg"
#define OR1K_TEST_PROG		"/sw/or1k_test"

Vbench *DUT  = new Vbench("Vbench");
wb_master *MASTER = new wb_master("wb_master");

char *or1ksim_argv[10];
int or1ksim_argc;

void xfer_finished(void)
{
	cout << "Xfer finished" << endl;
}

void *test_thread(void *arg)
{
	int ret;

	while(!(MASTER->master_ready()));

	generic_set_master(MASTER);

	ret = or1ksim_init(or1ksim_argc, or1ksim_argv, NULL, generic_read, generic_write);
	if (ret) {
		printf("or1ksim_init failed (%d)\n", ret);
	}

	ret = or1ksim_run(2);
	printf("or1ksim_run returned %d\n", ret);

	usleep(10);

	sc_stop();

	pthread_exit(0);
}

int sc_main(int argc, char* argv[])
{
	pthread_t th1;
	void *ret;

	sc_signal	< bool >	rst;
	sc_signal	< uint32_t >	wb_m2s_master_adr;
	sc_signal	< uint32_t >	wb_m2s_master_dat;
	sc_signal	< bool >	wb_m2s_master_we;
	sc_signal	< bool >	wb_m2s_master_cyc;
	sc_signal	< bool >	wb_m2s_master_stb;
	sc_signal	< uint32_t >	wb_m2s_master_cti;
	sc_signal	< uint32_t >	wb_m2s_master_bte;
	sc_signal	< uint32_t >	wb_s2m_master_dat;
	sc_signal	< bool >	wb_s2m_master_ack;
	sc_signal	< bool >	wb_s2m_master_err;
	sc_signal	< bool >	wb_s2m_master_rty;
	sc_signal	< uint32_t >	wb_m2s_master_sel;
	sc_signal	< bool >	sck;
	sc_signal	< bool >	ss;
	sc_signal	< bool >	mosi;
	sc_signal	< bool >	miso;

	printf("\n");

	// init trace dump
	Verilated::traceEverOn(true);
	VerilatedVcdC* tfp = new VerilatedVcdC;
	DUT->trace(tfp, 99);
	tfp->open("counter.vcd");

	or1ksim_argc = 4;
	or1ksim_argv[0] = NULL;
	or1ksim_argv[1] = (char *)"-f";

	if (!getenv("BUILD_ROOT")) {
		printf("ERROR: No BUILD_ROOT found in your environment\n");
		return -1;
	}

	or1ksim_argv[2] = (char *)malloc(strlen(getenv("BUILD_ROOT")) + strlen(OR1KSIM_CONF_FILE));
	strcpy(or1ksim_argv[2], getenv("BUILD_ROOT"));
	strcat(or1ksim_argv[2], OR1KSIM_CONF_FILE);

	or1ksim_argv[3] = (char *)malloc(strlen(getenv("BUILD_ROOT")) + strlen(OR1K_TEST_PROG));
	strcpy(or1ksim_argv[3], getenv("BUILD_ROOT"));
	strcat(or1ksim_argv[3], OR1K_TEST_PROG);

	sc_clock clk("clk", 10, SC_NS, 0.5);   // Create a clock signal

	MASTER->clk(clk);
	MASTER->rst(rst);

	MASTER->wb_adr(wb_m2s_master_adr);
	MASTER->wb_dat_i(wb_s2m_master_dat);
	MASTER->wb_we(wb_m2s_master_we);
	MASTER->wb_cyc(wb_m2s_master_cyc);
	MASTER->wb_stb(wb_m2s_master_stb);
	MASTER->wb_cti(wb_m2s_master_cti);
	MASTER->wb_bte(wb_m2s_master_bte);
	MASTER->wb_dat_o(wb_m2s_master_dat);
	MASTER->wb_ack(wb_s2m_master_ack);
	MASTER->wb_err(wb_s2m_master_err);
	MASTER->wb_rty(wb_s2m_master_rty);
	MASTER->wb_sel(wb_m2s_master_sel);

	DUT->wb_clk(clk);
	DUT->wb_rst(rst);

	DUT->wb_m2s_master_adr(wb_m2s_master_adr);
	DUT->wb_m2s_master_dat(wb_m2s_master_dat);
	DUT->wb_m2s_master_sel(wb_m2s_master_sel);
	DUT->wb_m2s_master_we (wb_m2s_master_we);
	DUT->wb_m2s_master_cyc(wb_m2s_master_cyc);
	DUT->wb_m2s_master_stb(wb_m2s_master_stb);
	DUT->wb_m2s_master_cti(wb_m2s_master_cti);
	DUT->wb_m2s_master_bte(wb_m2s_master_bte);
	DUT->wb_s2m_master_dat(wb_s2m_master_dat);
	DUT->wb_s2m_master_ack(wb_s2m_master_ack);
	DUT->wb_s2m_master_err(wb_s2m_master_err);
	DUT->wb_s2m_master_rty(wb_s2m_master_rty);

	DUT->sck(sck);
	DUT->ss(ss);
	DUT->mosi(mosi);
	DUT->miso(miso);

	sc_trace_file *fp;
	fp=sc_create_vcd_trace_file("wave");
	fp->set_time_unit(100, SC_PS);

	sc_trace(fp, clk, "clk");
	sc_trace(fp, rst, "rst");
	sc_trace(fp, wb_m2s_master_adr, "wb_m2s_master_adr");
	sc_trace(fp, wb_m2s_master_cyc, "wb_m2s_master_cyc");
	sc_trace(fp, wb_m2s_master_stb, "wb_m2s_master_stb");
	sc_trace(fp, wb_m2s_master_dat, "wb_m2s_master_dat");
	sc_trace(fp, wb_s2m_master_dat, "wb_s2m_master_dat");
	sc_trace(fp, wb_m2s_master_we, "wb_m2s_master_we");
	sc_trace(fp, wb_s2m_master_ack, "wb_s2m_master_ack");
	sc_trace(fp, wb_m2s_master_sel, "wb_m2s_master_sel");
	sc_trace(fp, sck, "sck");
	sc_trace(fp, ss, "ss");
	sc_trace(fp, mosi, "mosi");

	if (pthread_create (&th1, NULL, test_thread, NULL) < 0) {
		printf("pthread_create error for thread 1\n");
		exit (1);
	}

	sc_start();                // Run simulation

	sc_close_vcd_trace_file(fp);        // close(fp)

	(void)pthread_join (th1, &ret);

	tfp->close();

	return 0;                           // Return no errors
}
