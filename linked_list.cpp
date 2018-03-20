#include "base.h"
#include "example_list.h"

int main() {
	cl::CommandQueue queue;
	cl::Kernel kernel = initCL("linked_list.cl", queue);

	struct ExampleList list;
	initExampleList(&list);
	list.value = 5;
}
