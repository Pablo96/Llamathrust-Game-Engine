#pragma once
#include <log.h>

#define TEST_SUCCESS 0

#define START_TEST(name) unsigned long name(void* __ignored__) {

#define END_TEST }

#define LT_ADD_TEST(testName) __TestAdd(testName, #testName)

void __TestAdd(unsigned long (*func)(void*), const char* testName);

void LT_TestRun(void);
