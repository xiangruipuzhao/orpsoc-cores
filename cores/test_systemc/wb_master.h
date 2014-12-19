#include "systemc.h"
#include "wb_xfer.h"

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

	void do_poll_ack() {
		while (true) {
			wait();
			if (wb_ack && xfer_started) {
				e2.notify(SC_ZERO_TIME);
				xfer_started = 0;
			}
			local_ack = wb_ack;
		}
	}

	void do_xfer() {
		while (true) {
			wait(e1);
			cout << "@" << sc_time_stamp() <<" Triggering e1"<<endl;
			wb_cyc = local_xfer->cyc;
			wb_stb = local_xfer->stb;
			wb_adr = local_xfer->adr;
			wb_we  = local_xfer->we;
			wb_dat_o = local_xfer->dat_o;
			xfer_started = 1;
			wait(e2);
			wb_cyc = 0;
			wb_stb = 0;
			pthread_mutex_unlock(local_xfer->work);
			if (local_xfer->done)
				local_xfer->done();
		}
	}

	SC_CTOR(wb_master) {
		SC_THREAD(do_xfer);
		dont_initialize();
		sensitive << clk.pos();

		SC_THREAD(do_poll_ack);
		dont_initialize();
		sensitive << clk.pos();
	}

	void setup_xfer(struct wb_xfer *xfer) {
		local_xfer = xfer;
		cout << "@" << sc_time_stamp() << "wb.cyc = " << local_xfer->cyc << endl;
		e1.notify(SC_ZERO_TIME);
		pthread_mutex_lock(local_xfer->work);
	}
};
