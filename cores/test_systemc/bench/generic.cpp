#include <systemc.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include "wb_master.h"
#include "wb_xfer.h"
#include "or1ksim.h"

wb_master *master;

static inline uint32_t mask_to_sel(unsigned char mask[])
{
	return (!!(mask[0]) << 3) + (!!(mask[1]) << 2) + (!!(mask[2]) << 1) + !!(mask[3]);
}

uint32_t wb_read32(wb_master *wb_master, uint32_t addr, unsigned char mask[], void (*done)(void))
{
	wb_master->xfer.cyc = 1;
	wb_master->xfer.stb = 1;
	wb_master->xfer.adr = addr;
	wb_master->xfer.we  = 0;
	//wb_master->xfer.dat_o = data;
	wb_master->xfer.done = done;
	wb_master->xfer.busy = 1;
	wb_master->xfer.sel = mask_to_sel(mask);

	wb_master->start_xfer();

	pthread_mutex_lock(wb_master->xfer.mutex);
	while (wb_master->xfer.busy)
		pthread_cond_wait(wb_master->xfer.cond, wb_master->xfer.mutex);
	pthread_mutex_unlock(wb_master->xfer.mutex);

	return wb_master->xfer.dat_i;
}

void wb_write32(wb_master *wb_master, uint32_t addr, uint32_t data, unsigned char mask[], void (*done)(void))
{
	wb_master->xfer.cyc = 1;
	wb_master->xfer.stb = 1;
	wb_master->xfer.adr = addr;
	wb_master->xfer.we  = 1;
	wb_master->xfer.dat_o = data;
	wb_master->xfer.done = done;
	wb_master->xfer.busy = 1;
	wb_master->xfer.sel = mask_to_sel(mask);

	wb_master->start_xfer();

	pthread_mutex_lock(wb_master->xfer.mutex);
	while (wb_master->xfer.busy)
		pthread_cond_wait(wb_master->xfer.cond, wb_master->xfer.mutex);
	pthread_mutex_unlock(wb_master->xfer.mutex);
}

int generic_read(void *class_ptr, unsigned long int addr, unsigned char mask[], unsigned char rdata[], int data_len)
{
	uint32_t data;

	data = wb_read32(master, addr, mask, NULL);
	rdata[0] = (data >> 24) & 0xff;
	rdata[1] = (data >> 16) & 0xff;
	rdata[2] = (data >> 8) & 0xff;
	rdata[3] = data & 0xff;

	return 0;
}

int generic_write(void *class_ptr, unsigned long int addr, unsigned char mask[], unsigned char wdata[], int data_len)
{
	uint32_t data;

	data = (wdata[0] << 24);

	wb_write32(master, addr, data, mask, NULL);
	return 0;
}

void generic_set_master(wb_master *wb_master)
{
	master = wb_master;
}
