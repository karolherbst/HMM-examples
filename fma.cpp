#include <iostream>

#include "base.h"

int main() {
	cl::CommandQueue queue;
	cl::Kernel kernel = initCL("fma.cl", queue);
	cl_int ret;

	uint64_t res = 0;
	uint32_t a = 0x10;
	uint32_t b = 0x20;
	uint64_t c = 0x30;
	uint64_t size = 1;

	ret = kernel.setArg(0, &res);
	ret |= kernel.setArg(1, &a);
	ret |= kernel.setArg(2, &b);
	ret |= kernel.setArg(3, &c);
	ret |= kernel.setArg(4, size);
	if (ret) {
		std::cerr << "Failed to set args" << std::endl;
	}

	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(1), cl::NDRange(1), nullptr, nullptr);
	queue.finish();

	uint64_t result = a * b + c;
	if (res != result) {
		std::cerr << "Wrong result " << res << " != " << result << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
