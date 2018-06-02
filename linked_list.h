#include "defines.h"

struct LinkedList {
	struct LinkedList global* next;
};

static inline
void
linkedListInit(struct LinkedList global* list)
{
	list->next = NULL;
}

static inline
void
linkedListRemoveNext(struct LinkedList global* list)
{
	if (!list->next)
		return;

	struct LinkedList global* next = list->next;
	list->next = next->next;
	next->next = NULL;
}

static inline
void
linkedListInsert(struct LinkedList global* parent, struct LinkedList global* next)
{
	if (!next || !parent)
		return;

	next->next = parent->next;
	parent->next = next;
}
