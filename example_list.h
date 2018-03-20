#ifdef __OPENCL_C_VERSION__
# define global global
#else
# define global
#endif

struct ExampleList {
	global struct ExampleList *prev;
	global struct ExampleList *next;
	int value;
};

void initExampleList(struct ExampleList *);
