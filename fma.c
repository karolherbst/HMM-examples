#include <math.h>
#include <stdio.h>
#include <time.h>

#include "base.h"

int main() {
	srand(time(NULL));

	cl_context context;
	cl_command_queue queue;
	cl_kernel kernel;
	cl_device_svm_capabilities svm;
	cl_int ret;

	ret = initCL("fma.cl", &context, &kernel, &queue, &svm);
	if (ret)
		return ret;

	const uint64_t size = 10;
	const uint64_t byteSize = size * sizeof(float);
	float *res = malloc(byteSize);
	float *a = malloc(byteSize);
	float *b = malloc(byteSize);
	float *c = malloc(byteSize);
	float *correct_results = malloc(byteSize);

	for (int i = 0; i < size; ++i) {
		a[i] = (float)(rand()) / (float)(RAND_MAX / 100);
		b[i] = (float)(rand()) / (float)(RAND_MAX / 100);
		c[i] = (float)(rand()) / (float)(RAND_MAX / 100);
		res[i] = 0;
		correct_results[i] = fma(a[i], b[i], c[i]);
	}

	cl_mem outBuffer;
	if (svm & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) {
		ret  = clSetKernelArgSVMPointer(kernel, 0, res);
		ret |= clSetKernelArgSVMPointer(kernel, 1, a);
		ret |= clSetKernelArgSVMPointer(kernel, 2, b);
		ret |= clSetKernelArgSVMPointer(kernel, 3, c);
	} else {
		outBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, byteSize, NULL, &ret);
		cl_mem aBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, byteSize, NULL, &ret);
		cl_mem bBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, byteSize, NULL, &ret);
		cl_mem cBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, byteSize, NULL, &ret);

		ret = clEnqueueWriteBuffer(queue, aBuffer, 0, 0, byteSize, a, 0, NULL, NULL);
		ret = clEnqueueWriteBuffer(queue, bBuffer, 0, 0, byteSize, b, 0, NULL, NULL);
		ret = clEnqueueWriteBuffer(queue, cBuffer, 0, 0, byteSize, c, 0, NULL, NULL);

		ret  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &outBuffer);
		ret |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &aBuffer);
		ret |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &bBuffer);
		ret |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &cBuffer);
	}
	ret |= clSetKernelArg(kernel, 4, sizeof(size), &size);

	if (ret) {
		printf("Failed to set args\n");
		return ret;
	}

	const size_t global_size[3] = { size, 0, 0 };
	const size_t local_size[3] = { 1, 0, 0 };
	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_size, local_size, 0, NULL, NULL);

	if (!(svm & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM))
		clEnqueueReadBuffer(queue, outBuffer, 0, 0, byteSize, res, 0, NULL, NULL);
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
