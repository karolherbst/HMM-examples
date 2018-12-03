#include <float.h>
#include <tgmath.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "base.h"
#include "defines.h"

void normalize(float *in, float *out, int size)
{
	float w = 0.0;
	for (int i = 0; i < size; i++)
		w += (in[i] * in[i]);
	w = sqrt(w);

	for (int i = 0; i < size; i++)
		out[i] = in[i] / w;
}

int main() {
	srand(time(NULL));

	cl_context context;
	cl_command_queue queue;
	cl_kernel kernel;
	cl_device_svm_capabilities svm;
	cl_int ret;

	ret = initCL("normalize.cl", &context, &kernel, &queue, &svm);
	if (ret)
		return ret;

	const uint64_t size = 10;
	const uint64_t vsize = 4;
	const uint64_t byteSize = size * sizeof(float4);
	float4 * res = malloc(byteSize);
	float4 * a = malloc(byteSize);
	float4 * correct_results = malloc(byteSize);

	// some hard coded checks
	a[0][0] = INFINITY;
	a[0][1] = 546.54;
	a[0][2] = INFINITY;
	a[0][3] = 3.5;
	correct_results[0][0] = 0.7;
	correct_results[0][1] = 0.0;
	correct_results[0][2] = 0.7;
	correct_results[0][3] = 0.0;

	for (int i = 1; i < size; ++i) {
		for (int j = 1; j < vsize; ++j) {
			a[i][j] = (float)(rand()) / (float)(RAND_MAX / 100);
			res[i][j] = 0;
		}
		normalize(a[i], correct_results[i], vsize);
	}

	cl_mem outBuffer;
	if (svm & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) {
		ret  = clSetKernelArgSVMPointer(kernel, 0, res);
		ret |= clSetKernelArgSVMPointer(kernel, 1, a);
	} else {
		outBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, byteSize, NULL, &ret);
		cl_mem aBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, byteSize, NULL, &ret);

		ret = clEnqueueWriteBuffer(queue, aBuffer, 0, 0, byteSize, a, 0, NULL, NULL);

		ret  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &outBuffer);
		ret |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &aBuffer);
	}
	ret |= clSetKernelArg(kernel, 2, sizeof(size), &size);

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
		bool correct = true;
		for (int j = 0; j < vsize && correct; ++j)
			correct &= (fabs(res[i][j] - correct_results[i][j]) <= FLT_EPSILON);

		if (!correct) {
			printf("Wrong result %i: (%f", i, res[i][0]);
			for (int j = 1; j < vsize; ++j)
				printf(", %f", res[i][j]);
			printf(") != (%f", correct_results[i][0]);
			for (int j = 1; j < vsize; ++j)
				printf(", %f", correct_results[i][j]);
			printf(")\n");
			return EXIT_FAILURE;
		} else {
			printf("Result is correct! %i: normalize(%f", i, a[i][0]);
			for (int j = 1; j < vsize; ++j)
				printf(", %f", a[i][j]);
			printf(") = (%f", correct_results[i][0]);
			for (int j = 1; j < vsize; ++j)
				printf(", %f", correct_results[i][j]);
			printf(")\n");
		}
        }
	return EXIT_SUCCESS;
}
