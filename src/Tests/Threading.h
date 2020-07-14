#pragma once
#include "test_framework.h"
#include <Thread.h>
#include <CoreLib/Array.h>

static void function(void* param) {
  log_test("Initialized thread");

  Thread* this = LT_Thread_GetCurrent();

  LT_Thread_Sleep(this, 4000);

  log_test("Finished thread");
}

START_TEST(TestThreadSpawn)
  uint64 threads_count = 5;
  Array threads = LT_ArrayCreate(threads_count, sizeof(Thread));
  
  for (uint64 i = 0; i < threads_count; i++) {
  	Thread* thread = LT_Thread_Create(function, "thread1");
  	LT_ArraySetElement(&threads, i, thread);
    free(thread);
  }
  
  for (uint64 i = 0; i < threads_count; i++) {
  	Thread* thread = (Thread*) LT_ArrayGetElement(&threads, i);
  	LT_Thread_Join(thread);
  }
  
  LT_ArrayDestroy(&threads);
  return TEST_SUCCESS;
END_TEST
