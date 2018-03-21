#include "base.h"
#include "example_list.h"

int main() {
	cl_command_queue queue;
	cl_kernel kernel;
	cl_device_svm_capabilities svm;

	if (initCL("linked_list.cl", &kernel, &queue, &svm))
		return EXIT_FAILURE;

	struct ExampleList list;
	initExampleList(&list);
	list.value = 5;
}
