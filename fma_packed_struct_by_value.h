#include "defines.h"

struct FMAData {
	uint16_t size;
	struct {
		float a;
		float b;
		float c;
	} vals[10];
} __attribute__((packed));
