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

	sc_event e1, e2;
	bool local_ack;
	bool xfer_started;

	struct wb_xfer *local_xfer;
	thread_safe_event safe_ev;
	bool do_poll_ack_rdy;
	bool do_xfer_rdy;

	int master_ready() {
		return do_poll_ack_rdy & do_xfer_rdy;
	}

	void do_poll_ack() {
		while (true) {
			do_poll_ack_rdy = 1;
			wait();
			if (wb_ack && xfer_started) {
				pthread_mutex_lock(local_xfer->mutex);
				xfer_started = 0;
				pthread_mutex_unlock(local_xfer->mutex);
				e2.notify(SC_ZERO_TIME);
			}
			local_ack = wb_ack;
		}
	}

	void do_xfer() {
		while (true) {
			do_xfer_rdy = 1;
			wait(safe_ev.default_event());
			wb_cyc = local_xfer->cyc;
			wb_stb = local_xfer->stb;
			wb_adr = local_xfer->adr;
			wb_we  = local_xfer->we;
			wb_dat_o = local_xfer->dat_o;
			pthread_mutex_lock(local_xfer->mutex);
			xfer_started = 1;
			pthread_mutex_unlock(local_xfer->mutex);
			wait(e2);
			wb_cyc = 0;
			wb_stb = 0;

			pthread_mutex_lock(local_xfer->mutex);
			local_xfer->busy = 0;
			pthread_cond_signal(local_xfer->cond);
			pthread_mutex_unlock(local_xfer->mutex);

			if (local_xfer->done)
				local_xfer->done();
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
	}

	void setup_xfer(struct wb_xfer *xfer) {
		local_xfer = xfer;
		safe_ev.notify();
	}
};
