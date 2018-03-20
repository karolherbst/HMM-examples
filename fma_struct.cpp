#include <iostream>

#include "base.h"
#include "fma_struct.h"

int main() {
	cl::CommandQueue queue;
	cl::Kernel kernel = initCL("fma_struct.cl", queue);
	cl_int ret;

	constexpr uint64_t size = 2;
	uint64_t res[size];
	struct FMAData data[size];
	data[0].a = 0x10;
	data[0].b = 0x200;
	data[0].c = 0x30;
	data[1].a = 0x20;
	data[1].b = 0x300;
	data[1].c = 0x40;

	ret = kernel.setArg(0, &res);
	ret |= kernel.setArg(1, &data);
	ret |= kernel.setArg(2, size);
	if (ret) {
		std::cerr << "Failed to set args" << std::endl;
	}

	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(2), cl::NDRange(1), nullptr, nullptr);
	queue.finish();

	for (int i = 0; i < size; ++i) {
		uint64_t result = data[i].a * data[i].b + data[i].c;
		if (res[i] != result) {
			std::cerr << "Wrong result " << i << ": " << res << " != " << result << std::endl;
			return EXIT_FAILURE;
		}
        }
	std::cout << "Result is correct!" << std::endl;
	return EXIT_SUCCESS;
}
