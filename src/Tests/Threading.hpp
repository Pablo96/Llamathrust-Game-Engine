#pragma once
#include <stdio.h>
#include <string.h>

#include <Common.hpp>
#include <CoreLib/Array.hpp>
#include <Thread.hpp>
#include <ThreadPool.hpp>
#include <cstdlib>
#include <new>

#include "test_framework.hpp"

// in milliseconds
#define WAIT_TIME 1000

//--------------------------------------------------------------------------
// SIMPLE THREAD SPAWN
//--------------------------------------------------------------------------
static PROC_RETURN_T function(void *param) {
  LT::Thread *_this = reinterpret_cast<LT::Thread *>(param);
  log_test("Initialized thread");

  _this->Sleep(WAIT_TIME);

  log_test("Finished thread");
  return TEST_SUCCESS;
}

START_TEST(TestThreadSpawn)
uint64 threads_count = 5;
LT::Array threads = LT::Array(threads_count, sizeof(LT::Thread));

for (uint64 i = 0; i < threads_count; i++) {
  LT::Thread *thread = reinterpret_cast<LT::Thread *>(threads.GetElement(i));

  new (thread) LT::Thread(function, thread, nullptr, nullptr);
  thread->Start();
}

for (uint64 i = 0; i < threads_count; i++) {
  LT::Thread *thread = reinterpret_cast<LT::Thread *>(threads.GetElement(i));

  thread->Join(thread);
  delete thread;
}

return TEST_SUCCESS;
END_TEST

//--------------------------------------------------------------------------
// EXIT CODE
//--------------------------------------------------------------------------
#define EXIT_CODE_TEST 22

static PROC_RETURN_T functionExit(void *param) {
  LT::Thread *_this = reinterpret_cast<LT::Thread *>(param);

  _this->Sleep(WAIT_TIME);

  LT::Thread::Exit(EXIT_CODE_TEST);
}

START_TEST(TestThreadExitCode)
LT::Thread *thread = new LT::Thread(functionExit, nullptr, "thread1", nullptr);
thread->Start();

LT::Thread *_this = LT::Thread::GetCurrent();
_this->Join(thread);

int32 exitCode = thread->CaptureExitCode();

log_test("Thread exit code: %d", exitCode);

delete thread;
thread = nullptr;

return TEST_ASSERT(exitCode == EXIT_CODE_TEST);
END_TEST

//--------------------------------------------------------------------------
// LOCKING
//--------------------------------------------------------------------------
static PROC_RETURN_T functionLock(void *param) {
  LT::Thread *_this = reinterpret_cast<LT::Thread *>(param);

  _this->Sleep(LT_SECONDS(1));
  _this->Lock();

  printf("Thread %s: printing!\n", _this->GetName());

  _this->Unlock();
  return TEST_SUCCESS;
}

START_TEST(TestThreadLock)
uint32 count = 4;
LT::Array threads = LT::Array(count, sizeof(LT::Thread));
LT::ThreadLock *lock = new LT::ThreadLock();

for (uint32 i = 0; i < count; i++) {
  if (i > 99) break;

  char *name = static_cast<char *>(std::malloc(10));
  snprintf(name, 10, "Thread_%d", i);

  LT::Thread *thread = reinterpret_cast<LT::Thread *>(threads.GetElement(i));
  new (thread) LT::Thread(functionLock, nullptr, name, lock);
  thread->Start();
}

LT::Thread *_this = LT::Thread::GetCurrent();
for (uint32 i = 0; i < count; i++) {
  LT::Thread *thread = reinterpret_cast<LT::Thread *>(threads.GetElement(i));
  _this->Join(thread);
  delete thread;
  thread = nullptr;
}

delete lock;
lock = nullptr;

return TEST_SUCCESS;
END_TEST

//--------------------------------------------------------------------------
// THREADPOOL
//--------------------------------------------------------------------------
static PROC_RETURN_T Task1(void *_ignored) {
  log_test("Hello from Task1!");
  return TEST_SUCCESS;
}

static PROC_RETURN_T Task2(void *_ignored) {
  log_test("Hello from Task2!");
  return TEST_SUCCESS;
}

static PROC_RETURN_T Task3(void *_ignored) {
  log_test("Hello from Task3!");
  return TEST_SUCCESS;
}

static PROC_RETURN_T Task4(void *_ignored) {
  log_test("Hello from Task4!");
  return TEST_SUCCESS;
}

START_TEST(TestThreadPool)
LT::ThreadPool::Initialize(2, 4);

LT::ThreadPool::AddTask(Task1, nullptr);
LT::ThreadPool::AddTask(Task2, nullptr);
LT::ThreadPool::AddTask(Task3, nullptr);
LT::ThreadPool::AddTask(Task4, nullptr);

LT::Thread *_this = LT::Thread::GetCurrent();
_this->Sleep(LT_SECONDS(1));

LT::ThreadPool::Shutdown();
return TEST_SUCCESS;
END_TEST
