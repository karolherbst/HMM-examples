#include "fma_struct_ptrs.h"

kernel void test(global float *res, global struct FMAData *data, long size)
{
	int id = get_global_id(0);
	if (id >= size)
		return;
	res[id] = fma(*data[id].a, *data[id].b, *data[id].c);
}
