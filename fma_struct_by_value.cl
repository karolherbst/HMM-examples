#include "fma_struct_by_value.h"

kernel void test(global float *res, struct FMAData data)
{
	size_t id = get_global_id(0);
	if (id >= data.size)
		return;
	res[id] = fma(data.vals[id].a, data.vals[id].b, data.vals[id].c);
}
