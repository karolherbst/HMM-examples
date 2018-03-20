#include "fma_struct.h"

kernel void test(global long *res, global struct FMAData *data, long size)
{
	*res = data[0].a * data[0].b + data[0].c;
}
