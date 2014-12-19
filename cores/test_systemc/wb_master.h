#include "systemc.h"

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

	sc_event e1;

	void do_test1() {
		while (true) {
			wait(e1);
			cout << "@" << sc_time_stamp() <<" Triggering e1"<<endl;
			wb_cyc = 1;
			
		}
	}

	SC_CTOR(wb_master) {
		SC_THREAD(do_test1);
		dont_initialize();
		sensitive << clk.pos();
	}

	void drive_e1() {
		cout << "drived" <<endl;
		e1.notify(SC_ZERO_TIME);
	}
};
