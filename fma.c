#include <math.h>
#include <stdio.h>
#include <time.h>

#include "base.h"

int main() {
	srand(time(NULL));

	cl_command_queue queue;
	cl_kernel kernel;
	cl_device_svm_capabilities svm;
	cl_int ret;

	ret = initCL("fma.cl", &kernel, &queue, &svm);
	if (ret)
		return ret;

	const uint64_t size = 10;
	float *res = malloc(sizeof(float) * size);
	float *a = malloc(sizeof(float) * size);
	float *b = malloc(sizeof(float) * size);
	float *c = malloc(sizeof(float) * size);
	float *correct_results = malloc(sizeof(float) * size);

	for (int i = 0; i < size; ++i) {
		a[i] = (float)(rand()) / (float)(RAND_MAX / 100);
		b[i] = (float)(rand()) / (float)(RAND_MAX / 100);
		c[i] = (float)(rand()) / (float)(RAND_MAX / 100);
		res[i] = 0;
		correct_results[i] = fma(a[i], b[i], c[i]);
	}

	ret = clSetKernelArgSVMPointer(kernel, 0, res);
	ret |= clSetKernelArgSVMPointer(kernel, 1, a);
	ret |= clSetKernelArgSVMPointer(kernel, 2, b);
	ret |= clSetKernelArgSVMPointer(kernel, 3, c);
	ret |= clSetKernelArg(kernel, 4, sizeof(size), &size);
	if (ret) {
		printf("Failed to set args\n");
		return ret;
	}

	const size_t global_size[3] = { size, 0, 0 };
	const size_t local_size[3] = { 1, 0, 0 };
	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_size, local_size, 0, NULL, NULL);
	clFinish(queue);

	for (int i = 0; i < size; ++i) {
		float result = correct_results[i];
		if (res[i] != result) {
			printf("Wrong result %i: %f != %f\n", i, res[i], result);
			return EXIT_FAILURE;
		} else
			printf("Result is correct! %f * %f + %f = %f\n", a[i], b[i], c[i], result);
        }
	return EXIT_SUCCESS;
}
