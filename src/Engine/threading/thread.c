#include "Thread.h"
#include <Platform.h>
#include <stdlib.h>
#include <string.h> // memset

static uint64 threadIDCount = 0;

Thread* ConstructDummyThread(const void* platformObj, const uint16 size) {
  Thread tmp = {
    .name = NULL,
    .ID = LT_UINT64_MAX,
    .reservedSize = (uint16) size,
    .isValid = LT_TRUE
  };

  // Reserve heap mem and copy to heap
  Thread* threadObj = (Thread*) malloc(sizeof(Thread));
  memcpy(threadObj, &tmp, sizeof(Thread));
  memcpy(threadObj->reserved, platformObj, size);
  return threadObj;
}

Thread* ConstructThread(const void* platformObj, const uint16 size, const char* name) {
  Thread tmp = {
    .name = name,
    .ID = threadIDCount++,
    .reservedSize = (uint16) size,
    .isValid = LT_TRUE
  };

  // Reserve heap mem and copy to heap
  Thread* threadObj = (Thread*) malloc(sizeof(Thread));
  memcpy(threadObj, &tmp, sizeof(Thread));
  memcpy(threadObj->reserved, platformObj, size);
  return threadObj;
}

Thread* LT_Thread_Create(ThreadFuncWrapper func, void* data, const char* name) {
    return PlatformThreadCreate(func, data, name);
}

void LT_Thread_Join(const Thread* thread) {
  PlatformThreadJoin(thread);
}

void LT_Thread_JoinAll(const Thread** thread, const uint32 count) {
  
}

void LT_Thread_Sleep(const Thread* thread, const uint64 miliseconds) {
  PlatformThreadSleep(thread, miliseconds);
}

Thread* LT_Thread_GetCurrent() {
  return PlatformThreadGetCurrent();
}

void LT_Thread_Exit(const int32 exit_code) {
  PlatformThreadExit(exit_code);
}

void LT_Thread_ExitCode(Thread* thread) {
  PlatformThreadGetExitCode(thread);
}

void LT_Thread_Destroy(Thread* thread) {
  PlatformThreadDestroy(thread);
  memset(thread->reserved, 0, MAX_RESERVED_SIZE);
  thread->isValid = LT_FALSE;
}
