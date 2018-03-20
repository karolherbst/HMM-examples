#include <stddef.h>

#include "example_list.h"

void initExampleList(struct ExampleList *list)
{
	list->prev = NULL;
	list->next = NULL;
}
