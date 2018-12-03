#ifdef __OPENCL_C_VERSION__
# define global global
# define int64_t long
# define uint64_t ulong
# define int32_t int
# define uint32_t uint
# define int16_t short
# define uint16_t ushort
# define int8_t char
# define uint8_t uchar
#else
# define global
typedef float float2[2];
typedef float float3[3];
typedef float float4[4];
typedef float float8[8];
typedef float float16[16];
typedef double double2[2];
typedef double double3[3];
typedef double double4[4];
typedef double double8[8];
typedef double double16[16];
#endif

#ifndef NULL
# define NULL 0ull
#endif

#define container_of(ptr, type, member) ({                                           \
  char global* __mptr = (char global*)(ptr);                                         \
  type global* __tptr = ((type global*)(__mptr - __builtin_offsetof(type, member))); \
  ptr ? __tptr : 0;                                                                  \
})

#define member_size(type, member) sizeof(((type *)NULL)->member)
