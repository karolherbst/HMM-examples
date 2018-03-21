#include <cmath>

#include <iomanip>
#include <iostream>

#include "base.h"

int main() {
	srand(time(nullptr));

	cl::CommandQueue queue;
	cl::Kernel kernel = initCL("fma.cl", queue);
	cl_int ret;

	constexpr uint64_t size = 1000;
	float *res = new float[size];
	float *a = new float[size];
	float *b = new float[size];
	float *c = new float[size];
	float *correct_results = new float[size];

	for (int i = 0; i < size; ++i) {
		a[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 100);
		b[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 100);
		c[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 100);
		res[i] = 0;
		correct_results[i] = std::fma(a[i], b[i], c[i]);
	}

	ret = kernel.setArg(0, res);
	ret |= kernel.setArg(1, a);
	ret |= kernel.setArg(2, b);
	ret |= kernel.setArg(3, c);
	ret |= kernel.setArg(4, size);
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
			std::cout << std::fixed << std::setprecision(3) << "Result is correct! " << a[i] << " * " << b[i] << " + " << c[i] << " = " << result << std::endl;
        }
	return EXIT_SUCCESS;
}
