#include <math.h>
#include <stdio.h>
#include <time.h>

#include "base.h"
#include "fma_struct.h"

int main() {
	srand(time(NULL));

	cl_command_queue queue;
	cl_kernel kernel;
	cl_device_svm_capabilities svm;
	cl_int ret;

	ret = initCL("fma_struct.cl", &kernel, &queue, &svm);
	if (ret)
		return ret;

	const uint64_t size = 10;
	float *res = malloc(sizeof(float) * size);
	struct FMAData *data = malloc(sizeof(struct FMAData) *size);
	float *correct_results = malloc(sizeof(float) * size);

	for (int i = 0; i < size; ++i) {
		data[i].a = (float)(rand()) / (float)(RAND_MAX / 100);
		data[i].b = (float)(rand()) / (float)(RAND_MAX / 100);
		data[i].c = (float)(rand()) / (float)(RAND_MAX / 100);
		res[i] = 0;
		correct_results[i] = fma(data[i].a, data[i].b, data[i].c);
	}

	ret = clSetKernelArgSVMPointer(kernel, 0, res);
	ret |= clSetKernelArgSVMPointer(kernel, 1, data);
	ret |= clSetKernelArg(kernel, 2, sizeof(size), &size);
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
			printf("Result is correct! %f * %f + %f = %f\n", data[i].a, data[i].b, data[i].c, result);
        }
	return EXIT_SUCCESS;
}
