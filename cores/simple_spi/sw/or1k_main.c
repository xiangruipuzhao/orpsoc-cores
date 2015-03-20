#include <stdint.h>
#include <stdio.h>

#include "lib-gpio.h"
#include "io.h"

#define SIMPLE_SPI_BASE		0xa0000000

#define SPCR			0
#define SPSR			1
#define SPER			2
#define TREG			3
#define SS			4

int main(void)
{
	writeb(0x1, (void *)SIMPLE_SPI_BASE + 4);
	return 0;
}
