#include "wb_master.h"

int wb_master::master_ready(void)
{
	return do_poll_ack_rdy & do_xfer_rdy;
}

void wb_master::do_poll_ack(void)
{
	while (true) {
		do_poll_ack_rdy = 1;
		wait();
		if (wb_ack && xfer_started) {
			xfer.dat_i = wb_dat_i;
			pthread_mutex_lock(xfer.mutex);
			xfer_started = 0;
			pthread_mutex_unlock(xfer.mutex);
			e2.notify(SC_ZERO_TIME);
		}
	}
}

void wb_master::do_xfer(void)
{
	while (true) {
		do_xfer_rdy = 1;
		wait(safe_ev.default_event());
		wb_cyc = xfer.cyc;
		wb_stb = xfer.stb;
		wb_adr = xfer.adr;
		wb_we  = xfer.we;
		wb_dat_o = xfer.dat_o;
		wb_sel = xfer.sel;
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

void wb_master::start_xfer(void)
{
	safe_ev.notify();
}
