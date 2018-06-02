#include "linked_list_number.h"

static inline
struct NumberList global*
findLowest(struct NumberList global* head)
{
	struct NumberList global* lowHead = head;

	while (head && head->list.next) {
		struct NumberList global* o = NumberList(lowHead->list.next);
		struct NumberList global* n = NumberList(head->list.next);

		if (o->val > n->val)
			lowHead = head;

		head = n;
	}

	return lowHead;
}

kernel void
test(struct NumberList global* in)
{
	struct NumberList global* head = in;

	if (!head)
		return;

	while (head && head->list.next) {
		struct NumberList global* lowHead = findLowest(head);
		struct NumberList global* low = NumberList(lowHead->list.next);

		linkedListRemoveNext(&lowHead->list);
		linkedListInsert(&head->list, &low->list);

		head = low;
	}
}
