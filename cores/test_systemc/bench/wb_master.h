#include "systemc.h"
#include "wb_xfer.h"
#include "thread_safe_event.h"

SC_MODULE(wb_master) {

	sc_in	< bool >	clk;
	sc_in	< bool >	rst;

	sc_out	< uint32_t >	wb_adr;
	sc_in	< uint32_t >	wb_dat_i;
	sc_out	< bool >	wb_we;
	sc_out	< bool >	wb_cyc;
	sc_out	< bool >	wb_stb;
	sc_out	< uint32_t >	wb_cti;
	sc_out	< uint32_t >	wb_sel;
	sc_out	< uint32_t >	wb_bte;
	sc_out	< uint32_t >	wb_dat_o;
	sc_in	< bool >	wb_ack;
	sc_in	< bool >	wb_err;
	sc_in	< bool >	wb_rty;

	sc_event e2;
	bool xfer_started;

	struct wb_xfer xfer;
	thread_safe_event safe_ev;
	bool do_poll_ack_rdy;
	bool do_xfer_rdy;

	pthread_mutex_t mutex;
	pthread_cond_t cond;

	int master_ready();
	void do_poll_ack();
	void do_xfer();
	void start_xfer();

	SC_HAS_PROCESS(wb_master);

	wb_master(sc_module_name name_) : sc_module(name_), safe_ev("safe_ev") {
		SC_THREAD(do_xfer);
		dont_initialize();
		sensitive << clk.pos();

		SC_THREAD(do_poll_ack);
		dont_initialize();
		sensitive << clk.pos();

		do_poll_ack_rdy = 0;
		do_xfer_rdy = 0;

		pthread_mutex_init(&mutex, 0);
		pthread_cond_init(&cond, 0);

		xfer.mutex = &mutex;
		xfer.cond = &cond;
	}
};
