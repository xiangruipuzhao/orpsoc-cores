#include <systemc.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <Vgpio.h>
#include "wb_master.h"
#include "wb_xfer.h"
#include "or1ksim.h"

wb_master *master;

uint32_t mask_to_sel(unsigned char mask[])
{
	/*
	printf("mask[0] = %02x\n", mask[0]);
	printf("mask[1] = %02x\n", mask[1]);
	printf("mask[2] = %02x\n", mask[2]);
	printf("mask[3] = %02x\n", mask[3]);
	printf("sel = %x\n", (!!(mask[0]) << 3) + (!!(mask[1]) << 2) + (!!(mask[2]) << 1) + !!(mask[3]));
	* */
	return (!!(mask[0]) << 3) + (!!(mask[1]) << 2) + (!!(mask[2]) << 1) + !!(mask[3]);
}

uint32_t wb_read32(wb_master *wb_master, uint32_t addr, void (*done)(void))
{
	return 0;
}

void wb_write32(wb_master *wb_master, uint32_t addr, uint32_t data, unsigned char mask[], void (*done)(void))
{
	wb_master->xfer.cyc = 1;
	wb_master->xfer.stb = 1;
	wb_master->xfer.adr = addr;
	wb_master->xfer.we  = 0;
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
	unsigned long int address;
	unsigned long int data;
	int rw_size;
/*
	address = get_adr_from_mask(addr, mask);
	rw_size = get_rw_size(mask);

	switch (rw_size) {
	case 1:
		data = wb_read8(master, address, NULL);
		printf("READ8 : 0x%08lX -> %08X\n", address, data);
		break;
	case 2:
		data = wb_read16(master, address, NULL);
		printf("READ16: 0x%08lX -> %08X\n", address, data);
		break;
	case 4:
		data = wb_read32(master, address, NULL);
		printf("READ32: 0x%08lX -> %08X\n", address, data);
		break;
	}
*/
	/* TEST VALUE */
	data = 0x11223344;

	//set_data_from_mask(rdata, data, mask, rw_size);

	return 0;
}

int generic_write(void *class_ptr, unsigned long int addr, unsigned char mask[], unsigned char wdata[], int data_len)
{
	unsigned long int address;
	unsigned long int data;
	int rw_size;

	wb_write32(master, addr, data, mask, NULL);

	return 0;
}

void generic_set_master(wb_master *wb_master)
{
	master = wb_master;
}
