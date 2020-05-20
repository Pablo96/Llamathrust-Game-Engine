#pragma once
// signed types
typedef char        int8;
typedef short int   int16;
typedef int         int32;
typedef long long   int64;
// unsigned types
typedef unsigned char       uint8;
typedef unsigned short int  uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;
typedef uint8               byte;

// precision floating point
#ifdef LT_PRECISION_HIGH
typedef double decimal;
#else
typedef float decimal;
#endif
