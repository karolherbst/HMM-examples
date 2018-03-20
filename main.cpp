#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <fstream>
#include <iostream>

#include <CL/cl2.hpp>

#include "example_list.h"

constexpr uint32_t PLATFORM = 0;
constexpr uint32_t DEVICE = 0;

// taken from https://github.com/Dakkers/OpenCL-examples/blob/master/example00/main.cpp
int main() {
	std::vector<cl::Platform> all_platforms;
	cl::Platform::get(&all_platforms);

	if (all_platforms.empty()) {
		std::cout<<" No devices found. Check OpenCL installation!" << std::endl;
		return EXIT_FAILURE;
	}

	cl::Platform default_platform=all_platforms[PLATFORM];
	std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << std::endl;

	// get default device (CPUs, GPUs) of the default platform
	std::vector<cl::Device> all_devices;
	default_platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);
	if (all_devices.empty()) {
		std::cout<<" No devices found. Check OpenCL installation!" << std::endl;
		return EXIT_FAILURE;
	}

	cl::Device default_device=all_devices[DEVICE];
	std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << std::endl;

	cl::Context context({default_device});

	std::ifstream file("main.cl");
	if (!file.is_open()) {
		std::cerr << "Failed to open \"main.cl\"\n";
		return EXIT_FAILURE;
	}

	std::string source((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	cl::Program::Sources sources(1, source);

	cl_int ret;
	cl::Program program(context, sources, &ret);
	if (ret) {
		std::cerr << "Failed to create program from \"main.cl\"\n";
		return -ret;
	}

	ret = program.compile();
	if (ret == CL_BUILD_PROGRAM_FAILURE || ret == CL_COMPILE_PROGRAM_FAILURE) {
		std::string buildLog;
		program.getBuildInfo(default_device, CL_PROGRAM_BUILD_LOG, &buildLog);
		std::cerr << buildLog << std::endl;
	} else if (ret) {
		std::cerr << "Failed to compile \"main.cl\"\n";
		return -ret;
	}
	program = cl::linkProgram({ program }, nullptr, nullptr, nullptr, &ret);
	if (ret) {
		std::cerr << "Failed to link \"main.cl\"\n";
		return -ret;
	}

	struct ExampleList list;
//	initLinkedList(&list.list);
	list.value = 5;

	cl::Kernel kernel(program, "test", &ret);
	if (ret) {
		std::cerr << "Failed to create kernel" << std::endl;
		return -ret;
	}
}
