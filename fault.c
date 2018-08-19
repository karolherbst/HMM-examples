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

	ret = initCL("fault.cl", &context, &kernel, &queue, &svm);
	if (ret)
		return ret;

	const uint64_t size = 10;
	const uint64_t faultSize = 0x10;

	const uint64_t byteSize = sizeof(uint32_t) * size;
	const uint64_t faultByteSize = sizeof(uint32_t) * faultSize;

	uint32_t *a = malloc(byteSize);
	uint32_t *res = malloc(byteSize);
	uint32_t *faultBuffer = calloc(faultSize, 4);

	for (int i = 0; i < size; ++i) {
		a[i] = 0;//rand();
		res[i] = 0;
	}

	cl_mem outBuffer;
	ret  = clSetKernelArg(kernel, 0, sizeof(size), &size);
	if (svm & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) {
		ret |= clSetKernelArgSVMPointer(kernel, 1, res);
		ret |= clSetKernelArgSVMPointer(kernel, 2, a);
		ret |= clSetKernelArgSVMPointer(kernel, 3, faultBuffer);
	} else {
		outBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, byteSize, NULL, &ret);
		cl_mem aBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, byteSize, NULL, &ret);
		cl_mem fBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, faultByteSize, NULL, &ret);

		ret |= clEnqueueWriteBuffer(queue, aBuffer, 0, 0, byteSize, a, 0, NULL, NULL);
		ret |= clEnqueueWriteBuffer(queue, fBuffer, 0, 0, faultByteSize, faultBuffer, 0, NULL, NULL);

		ret |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &outBuffer);
		ret |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &aBuffer);
		ret |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &fBuffer);
	}

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

	for (int i = 0; i < size; ++i)
		printf("out[%02x]: 0x%08x\n", i, res[i]);

	for (int i = 0; i < faultSize; ++i)
		printf("fault[%02x]: 0x%08x\n", i, faultBuffer[i]);

	return EXIT_SUCCESS;
}
