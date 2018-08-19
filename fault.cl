kernel void test(long size, global uint *res, global uint *a, global uint *fault)
{
	size_t id = get_global_id(0);
//	if (id >= size)
//		return;
	res[id] = a[id] / a[id+1];
}
