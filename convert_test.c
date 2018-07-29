#include <math.h>
#include <stdbool.h>
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

	ret = initCL("convert_test.cl", &context, &kernel, &queue, &svm);
	if (ret)
		return ret;

	const uint32_t size32 = 40;
	const uint32_t size64 = 1;
	const uint64_t byteSize32 = size32 * sizeof(uint32_t);
	const uint64_t byteSize64 = size64 * sizeof(uint64_t);

	uint32_t *res32 = malloc(byteSize32);
	uint64_t *res64 = malloc(byteSize64);

	uint32_t *result32 = malloc(byteSize32);
	uint64_t *result64 = malloc(byteSize64);

	// double -> int: rte, rtz, rtp, rtn
	result32[0]  =  1;
	result32[1]  =  0;
	result32[2]  =  1;
	result32[3]  =  0;
	// close nextafter(1.0, 0) double -> float: rte, rtz, rtp, rtn
	result32[4]  =  0x3f7fffff;
	result32[5]  =  0x3f7fffff;
	result32[6]  =  0x3f800000;
	result32[7]  =  0x3f7fffff;
	// -double -> int: rte, rtz, rtp, rtn
	result32[8]  = -1;
	result32[9]  =  0;
	result32[10] =  0;
	result32[11] = -1;
	// +-0.5 +-1.5 -> int: rte
	result32[12] =  1;
	result32[13] =  2;
	result32[14] = -1;
	result32[15] = -2;
	// close 1.0 double -> float: rte, rtz, rtp, rtn
	result32[16] = 0x3f800000;
	result32[17] = 0x3f7fffff;
	result32[18] = 0x3f800000;
	result32[19] = 0x3f7fffff;
	// ULONG_MAX -> float: rte, rtz, rtp, rtn
	result32[20] = 0x5f800000;
	result32[21] = 0x5f7fffff;
	result32[22] = 0x5f800000;
	result32[23] = 0x5f7fffff;
	// ULONG_MIN -> float: rte, rtz, rtp, rtn
	result32[24] = 0x00000000;
	result32[25] = 0x00000000;
	result32[26] = 0x00000000;
	result32[27] = 0x00000000;
	// 9223371487098962000 -> float: rte, rtz, rtp, rtn
	result32[28] = 0x5effffff;
	result32[29] = 0x5effffff;
	result32[30] = 0x5f000000;
	result32[31] = 0x5effffff;
	// ULONG_MAX -> float: rte, rtz, rtp, rtn
	result32[32] = 0x5f000000;
	result32[33] = 0x5effffff;
	result32[34] = 0x5f000000;
	result32[35] = 0x5effffff;
	// ULONG_MIN -> float: rte, rtz, rtp, rtn
	result32[36] = 0xdf000000;
	result32[37] = 0xdf000000;
	result32[38] = 0xdf000000;
	result32[39] = 0xdf000000;

	result64[0] = 0;

	cl_mem outBuffer32 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, byteSize32, NULL, &ret);
	cl_mem outBuffer64 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, byteSize64, NULL, &ret);

	ret  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &outBuffer32);
	ret |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &outBuffer64);

	if (ret) {
		printf("Failed to set args\n");
		return ret;
	}

	const size_t global_size[3] = { 1, 0, 0 };
	const size_t local_size[3] = { 1, 0, 0 };
	ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_size, local_size, 0, NULL, NULL);
	if (ret) {
		printf("Failed to enqueue kernel\n");
		return ret;
	}

	clEnqueueReadBuffer(queue, outBuffer32, 0, 0, byteSize32, res32, 0, NULL, NULL);
	clEnqueueReadBuffer(queue, outBuffer64, 0, 0, byteSize64, res64, 0, NULL, NULL);
	clFinish(queue);

	bool error = false;
	for (int i = 0; i < size32; ++i) {
		if (result32[i] != res32[i]) {
			error = true;
			printf("Wrong result32 %i: %x != %x\n", i, result32[i], res32[i]);
		}
	}

	for (int i = 0; i < size64; ++i) {
		if (result64[i] != res64[i]) {
			error = true;
			printf("Wrong result64 %i: %lx != %lx\n", i, result64[i], res64[i]);
		}
	}

	if (!error)
		printf("No Errors!\n");

	return EXIT_SUCCESS;
}
