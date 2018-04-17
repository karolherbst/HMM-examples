__kernel void test(const __global float* M1,
                   uint m1width, uint m1height,
                   const __global float* M2,
                   uint m2width, uint m2height,
                   __global float* M3)
{
	uint x = get_global_id(0);
	if (x >= m2width)
		return;

	uint y = get_global_id(1);
	if (y >= m1height)
		return;

	float result = 0;
	for (int k = 0; k < m1width; ++k)
		result += M1[y * m1width + k] * M2[k * m2width + x];
	M3[y * m2width + x] = result;
}
