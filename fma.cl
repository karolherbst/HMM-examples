kernel void test(global long *res, global int *a, global int *b, global long *c, long size)
{
	*res = a[0] * b[0] + c[0];
}
