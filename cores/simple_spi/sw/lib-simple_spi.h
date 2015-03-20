#ifndef __GPIO_H
#define __GPIO_H

#define SPCR			0
#define SPSR			1
#define SPER			2
#define TREG			3
#define SS			4

void set_chipselect(uint32_t base, uint8_t val);

#endif
