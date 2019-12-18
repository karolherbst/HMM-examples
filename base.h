#include <CL/cl.h>

#define xstr(s) str(s)
#define str(s) #s

cl_int initCL(const char *filename, cl_context *, cl_kernel *, cl_command_queue *, cl_device_svm_capabilities*);
