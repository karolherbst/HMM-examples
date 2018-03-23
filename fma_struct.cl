#include "fma_struct.h"

kernel void test(global float *res, global struct FMAData *data, long size)
{
	size_t id = get_global_id(0);
	if (id >= size)
		return;
	res[id] = fma(data[id].a, data[id].b, data[id].c);
}
