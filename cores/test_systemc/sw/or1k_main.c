#include <stdint.h>
#include <stdio.h>

#include "lib-gpio.h"
#include "io.h"

#define GENERIC_PERIPHERAL	0xa0000000

int main(void)
{
	printf("### Starting GPIO test ###\n");
	writeb(0x11, (void *)GENERIC_PERIPHERAL);
	writew(0x1122, (void *)GENERIC_PERIPHERAL);
	writel(0x11223344, (void *)GENERIC_PERIPHERAL);

	writeb(0x11, (void *)(GENERIC_PERIPHERAL + 1));
	writeb(0x11, (void *)(GENERIC_PERIPHERAL + 2));
	writeb(0x11, (void *)(GENERIC_PERIPHERAL + 3));
	writew(0x1122, (void *)(GENERIC_PERIPHERAL + 2));

	printf("--> %x\n", readb((void *)GENERIC_PERIPHERAL));
	printf("--> %x\n", readw((void *)GENERIC_PERIPHERAL));
	printf("--> %x\n", readl((void *)GENERIC_PERIPHERAL));

	printf("--> %x\n", readb((void *)GENERIC_PERIPHERAL + 1));
	printf("--> %x\n", readb((void *)GENERIC_PERIPHERAL + 2));
	printf("--> %x\n", readb((void *)GENERIC_PERIPHERAL + 3));
	printf("--> %x\n", readw((void *)GENERIC_PERIPHERAL + 2));

	return 0;
}
