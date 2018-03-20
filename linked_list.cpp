#include "base.h"
#include "example_list.h"

int main() {
	cl::Kernel kernel = initCL("linked_list.cl");

	struct ExampleList list;
	initExampleList(&list);
	list.value = 5;
}
