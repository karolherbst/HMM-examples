#include <iostream>

#include "base.h"
#include "fma_struct.h"

int main() {
	cl::CommandQueue queue;
	cl::Kernel kernel = initCL("fma_struct.cl", queue);
	cl_int ret;

	uint64_t res = 0;
	struct FMAData data;
	data.a = 0x10;
	data.b = 0x20;
	data.c = 0x30;
	uint64_t size = 1;

	ret = kernel.setArg(0, &res);
	ret |= kernel.setArg(1, &data);
	ret |= kernel.setArg(2, size);
	if (ret) {
		std::cerr << "Failed to set args" << std::endl;
	}

	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(1), cl::NDRange(1), nullptr, nullptr);
	queue.finish();

	uint64_t result = data.a * data.b + data.c;
	if (res != result) {
		std::cerr << "Wrong result " << res << " != " << result << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Result is correct!" << std::endl;
	return EXIT_SUCCESS;
}
