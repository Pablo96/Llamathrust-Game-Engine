#pragma once
#include <log.hpp>

#ifdef LT_WINDOWS
typedef uint32 (*func_proto)(void *);
#define TEST_SUCCESS 0
#define PROC_RETURN_T uint64
#define START_TEST(name) unsigned long name(void *__ignored__) {
#define TEST_ASSERT(condition) !(condition)
#else
typedef void *(*func_proto)(void *);
#define TEST_SUCCESS (void *)0
#define PROC_RETURN_T void *
#define START_TEST(name) void *name(void *__ignored__) {
#define TEST_ASSERT(condition) reinterpret_cast<void *>(!(condition))
#endif

#define END_TEST }
#define SET_EXPECTED_RESULT(result_code) \
  reinterpret_cast<PROC_RETURN_T>(result_code)

#define LT_ADD_TEST(testName, expected_code) \
  __TestAdd(testName, #testName, expected_code)

void __TestAdd(func_proto func, const char *testName,
               PROC_RETURN_T expected_code);

uint64 LT_TestRun(void);
