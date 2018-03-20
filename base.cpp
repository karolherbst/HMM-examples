#include <fstream>
#include <iostream>

#include "base.h"

constexpr uint32_t PLATFORM = 0;
constexpr uint32_t DEVICE = 0;

// taken from https://github.com/Dakkers/OpenCL-examples/blob/master/example00/main.cpp
cl::Kernel initCL(const std::string &filename, cl::CommandQueue &queue) {
	std::vector<cl::Platform> all_platforms;
	cl::Platform::get(&all_platforms);

	if (all_platforms.empty()) {
		std::cout<<" No devices found. Check OpenCL installation!" << std::endl;
		throw EXIT_FAILURE;
	}

	cl::Platform default_platform=all_platforms[PLATFORM];
	std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << std::endl;

	// get default device (CPUs, GPUs) of the default platform
	std::vector<cl::Device> all_devices;
	default_platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);
	if (all_devices.empty()) {
		std::cout<<" No devices found. Check OpenCL installation!" << std::endl;
		throw EXIT_FAILURE;
	}

	cl::Device default_device=all_devices[DEVICE];
	std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << std::endl;

	cl::Context context({default_device});

	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open \"" << filename << "\"\n";
		throw EXIT_FAILURE;
	}

	std::string source((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	cl::Program::Sources sources(1, source);

	cl_int ret;
	cl::Program program(context, sources, &ret);
	if (ret) {
		std::cerr << "Failed to create program" << std::endl;
		throw -ret;
	}

	ret = program.compile();
	if (ret == CL_BUILD_PROGRAM_FAILURE || ret == CL_COMPILE_PROGRAM_FAILURE) {
		std::string buildLog;
		program.getBuildInfo(default_device, CL_PROGRAM_BUILD_LOG, &buildLog);
		std::cerr << buildLog << std::endl;
	} else if (ret) {
		std::cerr << "Failed to compile" << std::endl;
		throw -ret;
	}
	program = cl::linkProgram({ program }, nullptr, nullptr, nullptr, &ret);
	if (ret) {
		std::cerr << "Failed to link" << std::endl;
		throw -ret;
	}

	cl::Kernel kernel(program, "test", &ret);
	if (ret) {
		std::cerr << "Failed to create kernel" << std::endl;
		throw -ret;
	}

	queue = cl::CommandQueue(context, default_device, 0, &ret);
	if (ret) {
		std::cerr << "Failed to create Command Queue" << std::endl;
		throw -ret;
	}

	return kernel;
}
