#include "defines.h"

struct FMAData {
	uint32_t size;
	struct {
		float a;
		float b;
		float c;
	} vals[10];
};
