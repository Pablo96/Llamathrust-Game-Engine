/**
 * @file Common.h
 * @author Pablo Narvaja
 * @brief Here are all common types and defines.
 **/
#pragma once

// signed types
typedef char int8;
typedef short int int16;
typedef int int32;
typedef long long int64;
// unsigned types
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef uint8 byte;


// MAX and MIN values
#define LT_UINT64_MAX 0xFFFFFFFFFFFFFFFF

// precision floating point
#ifdef LT_PRECISION_HIGH
typedef double decimal;
#else
typedef float decimal;
#endif

#define MAKE_FN_NAME(prefix, subfix) prefix##subfix

#ifdef __GNUC__
  #ifdef __clang__
    #define LT_CLANG
  #else
    #define LT_GCC
  #endif
#elif defined(_MSC_VER)
  #define LT_VS
#else
  #error "Compiler not supported!"
#endif

#ifdef _WIN64
#define LT_WINDOWS
#elif defined(_WIN32)
#error "Windows 32bit is not supported!"
#elif defined(__ANDROID__)
#define LT_ANDROID
#error "ANDORID is not supported!"
#elif defined(__linux__)
#define LT_LINUX
#elif defined(__APPLE__)
#error "APPLE is not supported!"
#endif

#ifdef LT_WINDOWS
#ifdef LT_TEST_FRAMEWORK
#define LT_DEBUG_BREAK(error_code) LT::Thread::Exit(error_code)
#else
#define LT_DEBUG_BREAK(error_code) __debugbreak()
#endif
#else
#include <signal.h>
#define LT_DEBUG_BREAK(error_code) raise(SIGTRAP)
#endif

#ifdef LT_DEBUG
#define LT_ASSERT(positive_condition, msg, error_code)                         \
  if (!(positive_condition)) {                                                 \
    log_error(msg);                                                            \
    LT_DEBUG_BREAK(error_code);                                                \
  }
#else
#define LT_ASSERT(positive_condition, msg, error_code)
#endif

#ifdef LT_CLANG
#define LT_NORETURN [[noreturn]]
#elif defined(LT_VS)
#define LT_NORETURN __declspec(noreturn)
#endif

#define LT_MILISECONDS(x) x
#define LT_SECONDS(x) x * 1000
#define LT_MINUTES(x) x * 60 * 1000

#define LT_CREATE_ENUM_OPERATORS(clazz)\
inline int operator&(const clazz& left, const clazz& right) {\
    return static_cast<int>(left) >= static_cast<int>(right);}
