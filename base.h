#define CL_HPP_TARGET_OPENCL_VERSION 200
#include <CL/cl2.hpp>

cl::Kernel initCL(const std::string &filename, cl::CommandQueue&);
