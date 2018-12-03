#include <assert.h>
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

	ret = initCL("mat_mul.cl", &context, &kernel, &queue, &svm);
	if (ret)
		return ret;

	const uint32_t m1height = 14;
	const uint32_t m1width = 15;

	const uint32_t m2height = m1width;
	const uint32_t m2width = 9;

	assert(m1width == m2height);

	const uint32_t m3width = m2width;
	const uint32_t m3height = m1height;

	const uint32_t m1elements = m1width * m1height;
	const uint32_t m2elements = m2width * m2height;
	const uint32_t m3elements = m3width * m3height;

	const uint32_t m1Size = m1elements * sizeof(float);
	const uint32_t m2Size = m2elements * sizeof(float);
	const uint32_t m3Size = m3elements * sizeof(float);

	float *res = calloc(m3elements, sizeof(float));
	float *correct_results_fma = calloc(m3elements, sizeof(float));
	float *correct_results_mad = calloc(m3elements, sizeof(float));

	float *m1 = malloc(m1Size);
	float *m2 = malloc(m2Size);

	for (int i = 0; i < m1elements; ++i)
		m1[i] = (float)(rand()) / (float)(RAND_MAX / 100);

	for (int i = 0; i < m2elements; ++i)
		m2[i] = (float)(rand()) / (float)(RAND_MAX / 100);

	for (int i = 0; i < m3height; ++i) {
		for (int j = 0; j < m3width; ++j) {
			float result_mad = 0.0;
			float result_fma = 0.0;
			for (int k = 0; k < m1width; ++k) {
				result_mad += m1[i * m1width + k] * m2[k * m2width + j];
				result_fma = fma(m1[i * m1width + k], m2[k * m2width + j], result_fma);
			}
			correct_results_mad[i * m3width + j] = result_mad;
			correct_results_fma[i * m3width + j] = result_fma;
		}
	}

	cl_mem outBuffer;
	if (svm & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) {
		ret  = clSetKernelArgSVMPointer(kernel, 0, m1);
		ret |= clSetKernelArgSVMPointer(kernel, 3, m2);
		ret |= clSetKernelArgSVMPointer(kernel, 6, res);
	} else {
		outBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, m3Size, NULL, &ret);
		cl_mem aBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, m1Size, NULL, &ret);
		cl_mem bBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, m2Size, NULL, &ret);

		ret = clEnqueueWriteBuffer(queue, aBuffer, 0, 0, m1Size, m1, 0, NULL, NULL);
		ret = clEnqueueWriteBuffer(queue, bBuffer, 0, 0, m2Size, m2, 0, NULL, NULL);

		ret  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &aBuffer);
		ret |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &bBuffer);
		ret |= clSetKernelArg(kernel, 6, sizeof(cl_mem), &outBuffer);
	}
	ret |= clSetKernelArg(kernel, 1, sizeof(m1width), &m1width);
	ret |= clSetKernelArg(kernel, 2, sizeof(m1height), &m1height);
	ret |= clSetKernelArg(kernel, 4, sizeof(m2width), &m2width);
	ret |= clSetKernelArg(kernel, 5, sizeof(m2height), &m2height);

	if (ret) {
		printf("Failed to set args\n");
		return ret;
	}

	const size_t global_size[3] = { m3width, m3height, 0 };
	const size_t local_size[3] = { 1, 1, 0 };
	clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_size, local_size, 0, NULL, NULL);

	if (!(svm & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM))
		clEnqueueReadBuffer(queue, outBuffer, 0, 0, m3Size, res, 0, NULL, NULL);
	clFinish(queue);

	printf("\n\n");
	for (int i = 0; i < m1height; ++i) {
		printf("|");
		for (int j = 0; j < m1width; ++j) {
			printf(" %11.5f ", m1[i * m1width + j]);
		}
		printf("|\n");
	}
	printf("\n*\n\n");
	for (int i = 0; i < m2height; ++i) {
		printf("|");
		for (int j = 0; j < m2width; ++j) {
			printf(" %11.5f ", m2[i * m2width + j]);
		}
		printf("|\n");
	}
	printf("\n=\n\n");
	for (int i = 0; i < m3height; ++i) {
		printf("|");
		for (int j = 0; j < m3width; ++j) {
			float cpu_fma = correct_results_fma[i * m3width + j];
			float cpu_mad = correct_results_mad[i * m3width + j];
			float gpu = res[i * m3width + j];

			if (cpu_fma != gpu && cpu_mad != gpu)
				printf("!%11.5f ", res[i * m3width + j]);
			else
				printf(" %11.5f ", res[i * m3width + j]);
		}
		printf("|\n");
	}
	return EXIT_SUCCESS;
}
