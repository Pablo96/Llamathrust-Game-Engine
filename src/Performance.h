#pragma once
#define LT_MEAUSRE_PERFORMANCE

#include <time.h>
clock_t t;

#define LT_START_TIME() t = clock()
#define LT_END_TIME() t = clock() - t


#ifdef LT_MEAUSRE_PERFORMANCE
#define LT_MEASURE_FUNC(x)\
t = clock();\
x;\
t = clock() - t

#else
#define LT_MEASURE_FUNC(x) x;
#endif
