#include <systemc.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <Vgpio.h>
#include "wb_master.h"
#include "wb_xfer.h"
#include "or1ksim.h"

wb_master *master;

uint8_t wb_read8(wb_master *wb_master, uint32_t addr, void (*done)(void))
{
	return 0;
}

uint16_t wb_read16(wb_master *wb_master, uint32_t addr, void (*done)(void))
{
	return 0;
}

uint32_t wb_read32(wb_master *wb_master, uint32_t addr, void (*done)(void))
{
	return 0;
}

void wb_write8(wb_master *wb_master, uint32_t addr, uint8_t data, void (*done)(void))
{

}

void wb_write16(wb_master *wb_master, uint32_t addr, uint16_t data, void (*done)(void))
{

}

void wb_write32(wb_master *wb_master, uint32_t addr, uint32_t data, void (*done)(void))
{
	wb_master->xfer.cyc = 1;
	wb_master->xfer.stb = 1;
	wb_master->xfer.adr = addr;
	wb_master->xfer.we  = 0;
	wb_master->xfer.dat_o = data;
	wb_master->xfer.done = done;
	wb_master->xfer.busy = 1;

	wb_master->start_xfer();

	pthread_mutex_lock(wb_master->xfer.mutex);
	while (wb_master->xfer.busy)
		pthread_cond_wait(wb_master->xfer.cond, wb_master->xfer.mutex);
	pthread_mutex_unlock(wb_master->xfer.mutex);
}

int get_rw_size(unsigned char mask[])
{
	if (mask[0] == 0xff && mask[1] == 0xff && mask[2] == 0xff && mask[3] == 0xff)
		return 4;
	if (mask[0] == 0xff && mask[1] == 0xff && mask[2] == 0x00 && mask[3] == 0x00)
		return 2;
	if (mask[0] == 0x00 && mask[1] == 0x00 && mask[2] == 0xff && mask[3] == 0xff)
		return 2;
	if (mask[0] == 0x00 && mask[1] == 0x00 && mask[2] == 0x00 && mask[3] == 0xff)
		return 1;
	if (mask[0] == 0x00 && mask[1] == 0x00 && mask[2] == 0xff && mask[3] == 0x00)
		return 1;
	if (mask[0] == 0x00 && mask[1] == 0xff && mask[2] == 0x00 && mask[3] == 0x00)
		return 1;
	if (mask[0] == 0xff && mask[1] == 0x00 && mask[2] == 0x00 && mask[3] == 0x00)
		return 1;
	return 0;
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
	int rw_size;

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

	/* TEST VALUE */
	data = 0x11223344;

	set_data_from_mask(rdata, data, mask, rw_size);

	return 0;
}

int generic_write(void *class_ptr, unsigned long int addr, unsigned char mask[], unsigned char wdata[], int data_len)
{
	unsigned long int address;
	unsigned long int data;
	int rw_size;

	address = get_adr_from_mask(addr, mask);
	rw_size = get_rw_size(mask);
	data = get_dat_from_mask(wdata, mask, rw_size);

	switch (data_len) {
	case 1:
		wb_write8(master, address, data, NULL);
		printf("WRITE8 : %x at 0x%08lX\n", data, address);
		break;
	case 2:
		wb_write16(master, address, data, NULL);
		printf("WRITE16: %x at 0x%08lX\n", data, address);
		break;
	case 4:
		wb_write32(master, address, data, NULL);
		printf("WRITE32: %x at 0x%08lX\n", data, address);
		break;
	}

	return 0;
}

void generic_set_master(wb_master *wb_master)
{
	master = wb_master;
}
