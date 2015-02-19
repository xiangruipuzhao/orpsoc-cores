#ifndef __GPIO_H
#define __GPIO_H

void gpio_write(uint32_t base, uint8_t val);
uint8_t gpio_read(uint32_t base);
void gpio_set_dir(uint32_t base, uint8_t val);
uint8_t gpio_get_dir(uint32_t base, uint8_t val);

#endif
