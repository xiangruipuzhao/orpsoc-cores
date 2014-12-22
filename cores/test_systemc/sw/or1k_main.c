#include <stdint.h>
#include <stdio.h>

#define GENERIC_PERIPHERAL	0xa0000000

void main(void)
{
	volatile uint32_t *ptr = (volatile uint32_t *)GENERIC_PERIPHERAL;
	*ptr = 0x11223344;
	printf("------- or1k_main -----\n");
	printf("--> %x\n", *ptr);
}
