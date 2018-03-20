#include <time.h>

#include <cstdlib>

#include <iostream>

#include "base.h"
#include "fma_struct.h"

int main() {
	srand(time(nullptr));

	cl::CommandQueue queue;
	cl::Kernel kernel = initCL("fma_struct.cl", queue);
	cl_int ret;

	constexpr uint64_t size = 100;
	uint64_t res[size];
	struct FMAData data[size];

	for (int i = 0; i < size; ++i) {
		data[i].a = rand() % 0x200 + 1;
		data[i].b = rand() % 0x200 + 1;
		data[i].c = rand() % 0x200 + 1;
		res[i] = 0;
	}
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

	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(size), cl::NDRange(1), nullptr, nullptr);
	queue.finish();

	for (int i = 0; i < size; ++i) {
		uint64_t result = data[i].a * data[i].b + data[i].c;
		if (res[i] != result) {
			std::cerr << "Wrong result " << i << ": " << res << " != " << result << std::endl;
			return EXIT_FAILURE;
		} else
			std::cout << "Result is correct! " << data[i].a << " * " << data[i].b << " + " << data[i].c << " = " << result << std::endl;
        }
	return EXIT_SUCCESS;
}
