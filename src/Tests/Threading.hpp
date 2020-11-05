#pragma once
#include "test_framework.h"
#include <Common.h>
#include <CoreLib/Array.h>
#include <Thread.h>
#include <ThreadPool.h>
#include <stdio.h>
#include <string.h>

#define WAIT_TIME 1000

//--------------------------------------------------------------------------
// SIMPLE THREAD SPAWN
//--------------------------------------------------------------------------
static void function(void *param) {
  Thread *this = param;
  log_test("Initialized thread");

  LT_Thread_Sleep(this, WAIT_TIME);

  log_test("Finished thread");
}

START_TEST(TestThreadSpawn)
uint64 threads_count = 5;
Array threads = LT_ArrayCreate(threads_count, sizeof(Thread));

for (uint64 i = 0; i < threads_count; i++) {
  Thread *thread = LT_ArrayGetElement(&threads, i);

  LT_Thread_Create(thread, function, thread, NULL, NULL);
  LT_Thread_Start(thread);
}

for (uint64 i = 0; i < threads_count; i++) {
  Thread *thread = (Thread *)LT_ArrayGetElement(&threads, i);
  LT_Thread_Join(thread);
  LT_Thread_Destroy(thread);
}

LT_ArrayDestroy(&threads);
return TEST_SUCCESS;
END_TEST

//--------------------------------------------------------------------------
// EXIT CODE
//--------------------------------------------------------------------------
#define EXIT_CODE_TEST 22

static void functionExit(void *param) {
  Thread *this = param;

  LT_Thread_Sleep(this, WAIT_TIME);

  LT_Thread_Exit(EXIT_CODE_TEST);
}

START_TEST(TestThreadExitCode)
Thread *thread = LT_Thread_Create(NULL, functionExit, NULL, "thread1", NULL);
LT_Thread_Start(thread);

LT_Thread_Join(thread);
LT_Thread_ExitCode(thread);

int32 exitCode = thread->exitCode;
log_test("Thread exit code: %d", thread->exitCode);

LT_Thread_Destroy(thread);
free(thread);

return TEST_ASSERT(exitCode == EXIT_CODE_TEST);
END_TEST

//--------------------------------------------------------------------------
// LOCKING
//--------------------------------------------------------------------------
static void functionLock(void *param) {
  Thread *this = param;

  LT_Thread_Sleep(this, 1000);
  LT_ThreadLock_Lock(this->lock);

  printf("Thread %s: printing!\n", this->name);

  LT_ThreadLock_Unlock(this->lock);
}

START_TEST(TestThreadLock)
uint32 count = 4;
Array threads = LT_ArrayCreate(count, sizeof(Thread));
ThreadLock *lock = LT_ThreadLock_Create();

for (uint32 i = 0; i < count; i++) {
  if (i > 99)
    break;

  char *name = malloc(10);
  snprintf(name, 10, "Thread_%d", i);

  Thread *thread = LT_ArrayGetElement(&threads, i);
  LT_Thread_Create(thread, functionLock, NULL, name, lock);
  LT_Thread_Start(thread);
}

for (uint32 i = 0; i < count; i++) {
  Thread *thread = (Thread *)LT_ArrayGetElement(&threads, i);
  LT_Thread_Join(thread);
  LT_Thread_Destroy(thread);
}

LT_ArrayDestroy(&threads);
LT_ThreadLock_Destroy(lock);
free(lock);

return TEST_SUCCESS;
END_TEST

//--------------------------------------------------------------------------
// THREADPOOL
//--------------------------------------------------------------------------
static void Task1(void *_ignored) { log_test("Hello from Task1!"); }

static void Task2(void *_ignored) { log_test("Hello from Task2!"); }

static void Task3(void *_ignored) { log_test("Hello from Task3!"); }

static void Task4(void *_ignored) { log_test("Hello from Task4!"); }

START_TEST(TestThreadPool)
LT_ThreadPoolInitialize(2, 4);

LT_ThreadPoolAddTask(Task1, NULL);
LT_ThreadPoolAddTask(Task2, NULL);
LT_ThreadPoolAddTask(Task3, NULL);
LT_ThreadPoolAddTask(Task4, NULL);

Thread *this = LT_Thread_GetCurrent(NULL);
LT_Thread_Sleep(this, SECONDS(1));

LT_ThreadPoolShutdown();
return TEST_SUCCESS;
END_TEST
