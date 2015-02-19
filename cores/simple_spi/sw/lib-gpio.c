/*
* Copyright (c) 2015, Franck Jullien <franck.jullien@gmail.com>
* All rights reserved.
*
* Redistribution and use in source and non-source forms, with or without
* modification, are permitted provided that the following conditions are met:
* * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* * Redistributions in non-source form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
*
* THIS WORK IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* WORK, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdint.h>
#include "io.h"

#define IO_OFFSET	0
#define DIR_OFFSET	1

void gpio_write(uint32_t base, uint8_t val)
{
	writeb(val, (uint8_t *)(base + IO_OFFSET));
}

uint8_t gpio_read(uint32_t base)
{
	return readb((uint8_t *)(base + IO_OFFSET));
}

void gpio_set_dir(uint32_t base, uint8_t val)
{
	writeb(val, (uint8_t *)(base + DIR_OFFSET));
}

uint8_t gpio_get_dir(uint32_t base, uint8_t val)
{
	return readb((uint8_t *)(base + DIR_OFFSET));
}
