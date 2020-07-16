#pragma once
#include "test_framework.h"
#include <Thread.h>
#include <CoreLib/Array.h>

#define WAIT_TIME 1000
#define EXIT_CODE_TEST 22

static void function(void* param) {
  log_test("Initialized thread");

  Thread* this = LT_Thread_GetCurrent();

  LT_Thread_Sleep(this, WAIT_TIME);

  log_test("Finished thread");
}

static void functionExit(void* param) {
  log_test("Initialized thread");

  Thread* this = LT_Thread_GetCurrent();

  LT_Thread_Sleep(this, WAIT_TIME);

  LT_Thread_Exit(EXIT_CODE_TEST);
}

START_TEST(TestThreadSpawn)
  uint64 threads_count = 5;
  Array threads = LT_ArrayCreate(threads_count, sizeof(Thread));
  
  for (uint64 i = 0; i < threads_count; i++) {
  	Thread* thread = LT_Thread_Create(function, NULL, "thread1");
  	LT_ArraySetElement(&threads, i, thread);
    free(thread);
  }
  
  for (uint64 i = 0; i < threads_count; i++) {
  	Thread* thread = (Thread*) LT_ArrayGetElement(&threads, i);
  	LT_Thread_Join(thread);
    LT_Thread_Destroy(thread);
  }
  
  LT_ArrayDestroy(&threads);
  return TEST_SUCCESS;
END_TEST

START_TEST(TestThreadExitCode)
  Thread* thread = LT_Thread_Create(functionExit, NULL, "thread1");
  
  LT_Thread_Join(thread);
  LT_Thread_ExitCode(thread);

  log_test("Thread exit code: %d", thread->exitCode);

  LT_Thread_Destroy(thread);
  free(thread);

  return TEST_ASSERT(thread->exitCode == EXIT_CODE_TEST);
END_TEST
