#include "defines.h"

struct ExampleList {
	global struct ExampleList *prev;
	global struct ExampleList *next;
	int value;
};

void initExampleList(global struct ExampleList *);
