#ifdef __OPENCL_C_VERSION__
# define global global
#else
# define global
#endif

#ifndef NULL
# define NULL 0ull
#endif

#define container_of(ptr, type, member) ({                                           \
  char global* __mptr = (char global*)(ptr);                                         \
  type global* __tptr = ((type global*)(__mptr - __builtin_offsetof(type, member))); \
  ptr ? __tptr : 0;                                                                  \
})
