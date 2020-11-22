#pragma once
#include <log.hpp>

#define TEST_SUCCESS 0
#define TEST_ASSERT(condition) !(condition)

#define START_TEST(name) unsigned long name(void *__ignored__) {

#define END_TEST }

#define LT_ADD_TEST(testName, expected_code)                                   \
  __TestAdd(testName, #testName, expected_code)

void __TestAdd(unsigned long (*func)(void *), const char *testName,
               const int expected_code);

int LT_TestRun(void);
