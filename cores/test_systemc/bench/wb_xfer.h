#ifndef __WB_XFER
#define __WB_XFER

#include <stdint.h>

struct wb_xfer {
	uint32_t adr;
	uint32_t dat_i;
	bool we;
	bool cyc;
	bool stb;
	uint32_t cti;
	uint32_t bte;
	uint32_t dat_o;
	bool ack;
	bool err;
	bool rty;

	bool busy;
	void (*done)(void);
	pthread_mutex_t *mutex;
	pthread_cond_t *cond;
};

#endif
