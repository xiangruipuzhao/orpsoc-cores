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
	sc_out	< uint32_t >	wb_bte;
	sc_out	< uint32_t >	wb_dat_o;
	sc_in	< bool >	wb_ack;
	sc_in	< bool >	wb_err;
	sc_in	< bool >	wb_rty;

	sc_event e2;
	bool local_ack;
	bool xfer_started;

	struct wb_xfer xfer;
	thread_safe_event safe_ev;
	bool do_poll_ack_rdy;
	bool do_xfer_rdy;

	pthread_mutex_t mutex;
	pthread_cond_t cond;

	int master_ready() {
		return do_poll_ack_rdy & do_xfer_rdy;
	}

	void do_poll_ack() {
		while (true) {
			do_poll_ack_rdy = 1;
			wait();
			if (wb_ack && xfer_started) {
				pthread_mutex_lock(xfer.mutex);
				xfer_started = 0;
				pthread_mutex_unlock(xfer.mutex);
				e2.notify(SC_ZERO_TIME);
			}
			local_ack = wb_ack;
		}
	}

	void do_xfer() {
		while (true) {
			do_xfer_rdy = 1;
			wait(safe_ev.default_event());
			wb_cyc = xfer.cyc;
			wb_stb = xfer.stb;
			wb_adr = xfer.adr;
			wb_we  = xfer.we;
			wb_dat_o = xfer.dat_o;
			pthread_mutex_lock(xfer.mutex);
			xfer_started = 1;
			pthread_mutex_unlock(xfer.mutex);
			wait(e2);
			wb_cyc = 0;
			wb_stb = 0;

			pthread_mutex_lock(xfer.mutex);
			xfer.busy = 0;
			pthread_cond_signal(xfer.cond);
			pthread_mutex_unlock(xfer.mutex);

			if (xfer.done)
				xfer.done();
		}
	}

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

	void start_xfer() {
		safe_ev.notify();
	}
};
