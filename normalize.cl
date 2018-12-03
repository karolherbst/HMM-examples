kernel void test(global float4 *res, global float4 *a, long size)
{
	size_t id = get_global_id(0);
	if (id >= size)
		return;
	res[id] = normalize(a[id]);
}
