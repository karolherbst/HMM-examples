#include <CL/cl.h>

#define xstr(s) str(s)
#define str(s) #s

cl_int initCL(const char *filename, cl_context *, cl_kernel *, cl_command_queue *, cl_device_svm_capabilities*);
void *SVMAlloc(cl_context, cl_svm_mem_flags, size_t size, cl_uint alignment, cl_device_svm_capabilities);
void SVMFree(cl_context, void *, cl_device_svm_capabilities);
