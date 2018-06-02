#include "linked_list.h"

struct NumberList {
	struct LinkedList list;
	unsigned long val;
};

static inline
struct NumberList global*
NumberList(struct LinkedList global* list)
{
	return container_of(list, struct NumberList, list);
}
