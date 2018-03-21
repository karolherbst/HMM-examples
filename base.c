#include <stdio.h>
#include <sys/stat.h>

#include "base.h"

const uint32_t PLATFORM = 0;
const uint32_t DEVICE = 0;

cl_int initCL(const char *filename, cl_context *context, cl_kernel *kernel, cl_command_queue *queue, cl_device_svm_capabilities *svm) {
	cl_int ret;

	cl_platform_id *platforms = malloc(sizeof(*platforms) * 10);
	cl_uint num_platforms;
	ret = clGetPlatformIDs(10, platforms, &num_platforms);

	if (ret || !num_platforms) {
		printf("No platforms found. Check OpenCL installation!\n");
		return ret ? ret : -1;
	}

	cl_platform_id default_platform = platforms[PLATFORM];

	const size_t str_size = 100;
	char vendor[str_size];
	char name[str_size];

	ret = clGetPlatformInfo(default_platform, CL_PLATFORM_VENDOR, str_size, &vendor, NULL);
	if (ret) {
		printf("Couldn't get platform vendor\n");
		return ret;
	}
	printf("CL_PLATFORM_VENDOR: %s\n", vendor);

	ret = clGetPlatformInfo(default_platform, CL_PLATFORM_NAME, str_size, &name, NULL);
	if (ret) {
		printf("Couldn't get platform name\n");
		return ret;
	}
	printf("CL_PLATFORM_NAME: %s\n", name);

	cl_device_id devices[10];
	cl_uint num_devices;
	ret = clGetDeviceIDs(default_platform, CL_DEVICE_TYPE_GPU, 10, devices, &num_devices);

	if (ret || !num_devices) {
		printf("No devices found. Check OpenCL installation!\n");
		return ret ? ret : -1;
	}

	cl_device_id default_device = devices[DEVICE];

	char device_name[str_size];
	char device_vendor[str_size];
	char device_version[str_size];

	ret = clGetDeviceInfo(default_device, CL_DEVICE_NAME, str_size, &device_name, NULL);
	if (ret) {
		printf("Couldn't get device name\n");
		return ret;
	}
	printf("CL_DEVICE_NAME: %s\n", device_name);

	ret = clGetDeviceInfo(default_device, CL_DEVICE_VENDOR, str_size, &device_vendor, NULL);
	if (ret) {
		printf("Couldn't get device vendor\n");
		return ret;
	}
	printf("CL_DEVICE_VENDOR: %s\n", device_vendor);

	ret = clGetDeviceInfo(default_device, CL_DEVICE_VERSION, str_size, &device_version, NULL);
	if (ret) {
		printf("Couldn't get device version\n");
		return ret;
	}
	printf("CL_DEVICE_VERSION: %s\n", device_version);

	ret = clGetDeviceInfo(default_device, CL_DEVICE_SVM_CAPABILITIES, sizeof(*svm), svm, NULL);
	if (ret) {
		printf("Failed to check SVM support!\n");
		*svm = 0;
	}
	printf("CL_DEVICE_SVM_COARSE_GRAIN_BUFFER: %i\n", *svm & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER ? 1 : 0);
	printf("CL_DEVICE_SVM_FINE_GRAIN_BUFFER: %i\n", *svm & CL_DEVICE_SVM_FINE_GRAIN_BUFFER ? 1 : 0);
	printf("CL_DEVICE_SVM_FINE_GRAIN_SYSTEM: %i\n", *svm & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM ? 1 : 0);
	printf("CL_DEVICE_SVM_FINE_ATOMICS: %i\n", *svm & CL_DEVICE_SVM_ATOMICS ? 1 : 0);

	*context = clCreateContext(NULL, 1, &default_device, NULL, NULL, &ret);
	if (ret) {
		printf("Failed to create context!\n");
		return ret;
	}

	FILE *fd = fopen(filename, "r");
	if (!fd) {
		printf("Failed to open file %s\n", filename);
		return -1;
	}

	struct stat st;
	stat(filename, &st);
	if (!st.st_size) {
		printf("Kernel source file is empty\n");
		return -1;
	}

	char *file_buffer = malloc(st.st_size);
	size_t bytes_read = fread(file_buffer, sizeof(char), st.st_size, fd);

	fclose(fd);

	if (bytes_read != st.st_size) {
		printf("Failed to read kernel source file\n");
		free(file_buffer);
		return -1;
	}

	cl_program program = clCreateProgramWithSource(*context, 1, (const char **)&file_buffer, NULL, NULL);
	free(file_buffer);

	ret = clBuildProgram(program, 1, &default_device, NULL, NULL, NULL);
	if (ret == CL_BUILD_PROGRAM_FAILURE || ret == CL_COMPILE_PROGRAM_FAILURE) {
		char *build_log = malloc(sizeof(char) * 10000);
		clGetProgramBuildInfo(program, default_device, CL_PROGRAM_BUILD_LOG, 10000, build_log, NULL);
		printf("%s\n", build_log);
		free(build_log);
		return ret;
	} else if (ret) {
		printf("Failed to compile kernel\n");
		return ret;
	}

	*kernel = clCreateKernel(program, "test", &ret);
	if (ret) {
		printf("Failed to create kernel!\n");
		return -ret;
	}

	*queue = clCreateCommandQueue(*context, default_device, 0, &ret);
	if (ret) {
		printf("Failed to create Command Queue!\n");
		return ret;
	}

	return 0;
}
