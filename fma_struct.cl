#include "fma_struct.h"

kernel void test(global long *res, global struct FMAData *data, long size)
{
	int id = get_global_id(0);
	if (id >= size)
		return;
	res[id] = data[id].a * data[id].b + data[id].c;
}
