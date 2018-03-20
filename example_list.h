#include "defines.h"

struct ExampleList {
	global struct ExampleList *prev;
	global struct ExampleList *next;
	int value;
};

#ifdef __cplusplus
extern "C" {
#endif

void initExampleList(global struct ExampleList *);

#ifdef __cplusplus
}
#endif
