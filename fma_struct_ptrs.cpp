#include <time.h>

#include <cmath>
#include <cstdlib>

#include <iomanip>
#include <iostream>

#include "base.h"
#include "fma_struct_ptrs.h"

int main() {
	srand(time(nullptr));

	cl::CommandQueue queue;
	cl::Kernel kernel = initCL("fma_struct_ptrs.cl", queue);
	cl_int ret;

	constexpr uint64_t size = 1000;
	float *res = new float[size];
	struct FMAData *data = new struct FMAData[size];
	float *correct_results = new float[size];
	float (*heap)[3] = new float[size][3];

	for (int i = 0; i < size; ++i) {
		heap[i][0] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 100);
		heap[i][1] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 100);
		heap[i][2] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 100);

		data[i].a = &heap[i][0];
		data[i].b = &heap[i][1];
		data[i].c = &heap[i][2];

		res[i] = 0;
		correct_results[i] = std::fma(heap[i][0], heap[i][1], heap[i][2]);
	}

	ret = kernel.setArg(0, res);
	ret |= kernel.setArg(1, data);
	ret |= kernel.setArg(2, size);
	if (ret) {
		std::cerr << "Failed to set args" << std::endl;
	}

	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(size), cl::NDRange(1), nullptr, nullptr);
	queue.finish();

	for (int i = 0; i < size; ++i) {
		float result = correct_results[i];
		if (std::fabs(res[i] - result) > std::numeric_limits<float>::epsilon()) {
			std::cerr << std::fixed << std::setprecision(15) << "Wrong result " << i << ": " << res[i] << " != " << result << std::endl;
			return EXIT_FAILURE;
		} else
			std::cout << std::fixed << std::setprecision(3) << "Result is correct! " << *data[i].a << " * " << *data[i].b << " + " << *data[i].c << " = " << result << std::endl;
        }

	return EXIT_SUCCESS;
}
