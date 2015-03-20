#include <stdint.h>
#include <stdio.h>

#include "lib-simple_spi.h"
#include "io.h"

#define SIMPLE_SPI_BASE		0xa0000000

int main(void)
{
	set_chipselect(SIMPLE_SPI_BASE, 1);
	set_chipselect(SIMPLE_SPI_BASE, 0);
	set_chipselect(SIMPLE_SPI_BASE, 1);
	return 0;
}
