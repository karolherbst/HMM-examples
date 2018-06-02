#include <math.h>
#include <stdio.h>
#include <time.h>

#include "base.h"
#include "linked_list_number.h"

int main() {
	srand(time(NULL));

	cl_context context;
	cl_command_queue queue;
	cl_kernel kernel;
	cl_device_svm_capabilities svm;
	cl_int ret;

	ret = initCL("linked_list_insertion_sort.cl", &context, &kernel, &queue, &svm);
	if (ret)
		return ret;

	if (!(svm & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM)) {
		printf("No CL_DEVICE_SVM_FINE_GRAIN_SYSTEM support!\n");
		return EXIT_FAILURE;
	}

	const uint32_t size = 10;
	struct NumberList *head = malloc(sizeof(*head));
	linkedListInit(&head->list);
	head->val = 0;

	struct NumberList *cur = head;
	for (int i = 1; i < size; ++i) {
		struct NumberList *e = malloc(sizeof(*e));
		linkedListInit(&e->list);
		e->val = rand() % 0x100;
		linkedListInsert(&cur->list, &e->list);
	}

	ret  = clSetKernelArgSVMPointer(kernel, 0, head);
	if (ret) {
		printf("Failed to set args\n");
		return ret;
	}

	printf("print list:");
	cur = head;
	while (cur) {
		printf(" %li", cur->val);
		cur = container_of(cur->list.next, struct NumberList, list);
	}

	printf("\n");

	const size_t global_size[3] = { 1, 0, 0 };
	const size_t local_size[3] = { 1, 0, 0 };

	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_size, local_size, 0, NULL, NULL);
	clFinish(queue);

	printf("print list:");
	cur = head;
	while (cur) {
		printf(" %li", cur->val);
		cur = container_of(cur->list.next, struct NumberList, list);
	}
	printf("\n");

	return EXIT_SUCCESS;
}
