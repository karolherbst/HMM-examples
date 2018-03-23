kernel void test(global float *res, global float *a, global float *b, global float *c, long size)
{
	size_t id = get_global_id(0);
	if (id >= size)
		return;
	res[id] = fma(a[id], b[id], c[id]);
}
