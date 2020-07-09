#include "thread.h"
#include <Platform.h>
#include <stdlib.h>
#include <string.h> // memset

static uint64 threadIDCount = 0;

Thread* ConstructThread(const void* platformObj, const uint16 size, const char* name) {
  Thread tmp = {
    .name = name,
    .ID = threadIDCount++,
    .reservedSize = (uint16) size
  };

  // Reserve heap mem and copy to heap
  Thread* threadObj = (Thread*) malloc(sizeof(Thread));
  memcpy(threadObj, &tmp, sizeof(Thread));
  memcpy(threadObj->reserved, platformObj, size);
  return threadObj;
}

Thread* LT_Thread_Create(ThreadFuncWrapper func, const char* name) {
    return PlatformThreadCreate(func, NULL, name);
}

void LT_Thread_Join(const Thread* thread) {
  PlatformThreadJoin(thread);
}

void LT_Thread_Sleep(const Thread* thread, const uint64 miliseconds) {
  PlatformThreadSleep(thread, miliseconds);
}
