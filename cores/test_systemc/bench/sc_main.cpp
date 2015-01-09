#include <systemc.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include "/home/franck/openrisc/upstream/fusesoc/build/test_systemc/sim-verilator/obj_dir/Vgpio.h"
#include "wb_master.h"
#include "wb_xfer.h"
#include "or1ksim.h"

#define OR1KSIM_CONF_FILE	"/sw/or1ksim.cfg"
#define OR1K_TEST_PROG		"/sw/or1k_test"

Vgpio *DUT  = new Vgpio("Vgpio");
wb_master *MASTER = new wb_master("wb_master");

char *or1ksim_argv[10];
int or1ksim_argc;

void xfer_finished(void)
{
	cout << "Xfer finished" << endl;
}

uint8_t wb_read8(uint32_t addr, void (*done)(void))
{
	return 0;
}

uint16_t wb_read16(uint32_t addr, void (*done)(void))
{
	return 0;
}

uint32_t wb_read32(uint32_t addr, void (*done)(void))
{
	return 0;
}

void wb_write8(uint32_t addr, uint8_t data, void (*done)(void))
{

}

void wb_write16(uint32_t addr, uint16_t data, void (*done)(void))
{

}

void wb_write32(uint32_t addr, uint32_t data, void (*done)(void))
{
	MASTER->xfer.cyc = 1;
	MASTER->xfer.stb = 1;
	MASTER->xfer.adr = addr;
	MASTER->xfer.we  = 0;
	MASTER->xfer.dat_o = data;
	MASTER->xfer.done = done;
	MASTER->xfer.busy = 1;

	MASTER->start_xfer();

	pthread_mutex_lock(MASTER->xfer.mutex);
	while (MASTER->xfer.busy)
		pthread_cond_wait(MASTER->xfer.cond, MASTER->xfer.mutex);
	pthread_mutex_unlock(MASTER->xfer.mutex);
}

unsigned long int get_adr_from_mask(unsigned long int addr, unsigned char mask[])
{
	if (mask[0] == 0xff)
		return addr;

	if (mask[1] == 0xff)
		return addr + 1;

	if (mask[2] == 0xff)
		return addr + 2;

	if (mask[3] == 0xff)
		return addr + 3;

	return addr;
}

unsigned long int get_dat_from_mask(unsigned char wdata[], unsigned char mask[], int data_len)
{
	if (data_len == 4)
		return (wdata[0] << 24) + (wdata[1] << 16) + (wdata[2] << 8) + wdata[3];

	if (data_len == 2) {
		if (mask[0] == 0xff)
			return (wdata[0] << 8) + wdata[1];
		else
			return (wdata[2] << 8) + wdata[3];
	}

	if (data_len == 1) {
		if (mask[0] == 0xff)
			return wdata[0];
		if (mask[1] == 0xff)
			return wdata[1];
		if (mask[2] == 0xff)
			return wdata[2];
		if (mask[3] == 0xff)
			return wdata[3];
	}

	return 0;
}

int set_data_from_mask(unsigned char *rdata, unsigned long data, unsigned char mask[], int data_len)
{
	if (data_len == 4) {
		rdata[0] = (data >> 24) & 0xff;
		rdata[1] = (data >> 16) & 0xff;
		rdata[2] = (data >> 8) & 0xff;
		rdata[3] = data & 0xff;
		return 0;
	}

	if (data_len == 2) {
		if (mask[0] == 0xff) {
			rdata[0] = (data >> 24) & 0xff;
			rdata[1] = (data >> 16) & 0xff;
			return 0;
		} else {
			rdata[2] = (data >> 8) & 0xff;
			rdata[3] = data & 0xff;
			return 0;
		}
	}

	if (data_len == 1) {
		if (mask[0] == 0xff) {
			rdata[0] = (data >> 24) & 0xff;
			return 0;
		}
		if (mask[1] == 0xff) {
			rdata[1] = (data >> 16) & 0xff;
			return 0;
		}
		if (mask[2] == 0xff) {
			rdata[2] = (data >> 8) & 0xff;
			return 0;
		}
		if (mask[3] == 0xff) {
			rdata[3] = data & 0xff;
			return 0;
		}
	}

	return 0;
}

int generic_read(void *class_ptr, unsigned long int addr, unsigned char mask[], unsigned char rdata[], int data_len)
{
	unsigned long int address;
	unsigned long int data;

	address = get_adr_from_mask(addr, mask);

	switch (data_len) {
	case 1:
		data = wb_read8(address, NULL);
		printf("READ8 : 0x%08lX -> %08X\n", address, data);
		break;
	case 2:
		data = wb_read16(address, NULL);
		printf("READ16: 0x%08lX -> %08X\n", address, data);
		break;
	case 4:
		data = wb_read32(address, NULL);
		printf("READ32: 0x%08lX -> %08X\n", address, data);
		break;
	}

	/* TEST VALUE */
	data = 0x11223344;

	set_data_from_mask(rdata, data, mask, data_len);

	return 0;
}

int generic_write(void *class_ptr, unsigned long int addr, unsigned char mask[], unsigned char wdata[], int data_len)
{
	unsigned long int address;
	unsigned long int data;

	address = get_adr_from_mask(addr, mask);
	data = get_dat_from_mask(wdata, mask, data_len);

	switch (data_len) {
	case 1:
		wb_write8(address, data, NULL);
		printf("WRITE8 : %x at 0x%08lX\n", data, address);
		break;
	case 2:
		wb_write16(address, data, NULL);
		printf("WRITE16: %x at 0x%08lX\n", data, address);
		break;
	case 4:
		wb_write32(address, data, NULL);
		printf("WRITE32: %x at 0x%08lX\n", data, address);
		break;
	}

	return 0;
}

void *test_thread(void *arg)
{
	int ret;

	while(!(MASTER->master_ready()));

	ret = or1ksim_init(or1ksim_argc, or1ksim_argv, NULL, generic_read, generic_write);
	if (ret) {
		printf("or1ksim_init failed (%d)\n", ret);
	}

	ret = or1ksim_run(2);
	printf("or1ksim_run returned %d\n", ret);
/*
	while(!(MASTER->master_ready()));

	usleep(10);

	wb_write32(0x5588, 0x65, NULL);

	wb_write32(0x11223344, 0x998877EE, xfer_finished);
*/
	usleep(10);

	sc_stop();

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

	or1ksim_argc = 4;
	or1ksim_argv[0] = NULL;
	or1ksim_argv[1] = "-f";

	or1ksim_argv[2] = (char *)malloc(strlen(getenv("BUILD_ROOT")) + strlen(OR1KSIM_CONF_FILE));
	strcpy(or1ksim_argv[2], getenv("BUILD_ROOT"));
	strcat(or1ksim_argv[2], OR1KSIM_CONF_FILE);

	or1ksim_argv[3] = (char *)malloc(strlen(getenv("BUILD_ROOT")) + strlen(OR1K_TEST_PROG));
	strcpy(or1ksim_argv[3], getenv("BUILD_ROOT"));
	strcat(or1ksim_argv[3], OR1K_TEST_PROG);

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
	sc_trace(fp, wb_dat_o, "wb_dat_o");             // Add signals to trace file
	sc_trace(fp, wb_we, "wb_we");             // Add signals to trace file
	sc_trace(fp, wb_ack, "wb_ack");             // Add signals to trace file

	if (pthread_create (&th1, NULL, test_thread, NULL) < 0) {
		printf("pthread_create error for thread 1\n");
		exit (1);
	}

	sc_start();                // Run simulation

	sc_close_vcd_trace_file(fp);        // close(fp)

	(void)pthread_join (th1, &ret);

	return 0;                           // Return no errors
}
