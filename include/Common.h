/**
 * @file Common.h
 * @author Pablo Narvaja
 * @brief Here are all common types and defines.
 **/
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

// boolean
typedef byte               bool;
#define LT_FALSE 0
#define LT_TRUE 1
#define NULL (void*)0


// precision floating point
#ifdef LT_PRECISION_HIGH
typedef double decimal;
#else
typedef float decimal;
#endif

#define MAKE_FN_NAME(prefix, subfix) prefix ## subfix

