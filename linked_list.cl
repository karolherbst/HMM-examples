#include "example_list.h"

kernel void test(global int* out, global struct ExampleList* list)
{
	*out = list->value;
}
