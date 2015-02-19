#ifndef __GENERIC_H
#define __GENERIC_H

#include <stdint.h>

uint8_t wb_read8(uint32_t addr, void (*done)(void));
uint16_t wb_read16(uint32_t addr, void (*done)(void));
uint32_t wb_read32(uint32_t addr, void (*done)(void));
void wb_write8(uint32_t addr, uint8_t data, void (*done)(void));
void wb_write16(uint32_t addr, uint16_t data, void (*done)(void));
void wb_write32(uint32_t addr, uint32_t data, void (*done)(void));
int get_rw_size(unsigned char mask[]);
unsigned long int get_adr_from_mask(unsigned long int addr, unsigned char mask[]);
unsigned long int get_dat_from_mask(unsigned char wdata[], unsigned char mask[], int data_len);
int set_data_from_mask(unsigned char *rdata, unsigned long data, unsigned char mask[], int data_len);
int generic_read(void *class_ptr, unsigned long int addr, unsigned char mask[], unsigned char rdata[], int data_len);
int generic_write(void *class_ptr, unsigned long int addr, unsigned char mask[], unsigned char wdata[], int data_len);
void generic_set_master(wb_master *wb_master);

#endif
