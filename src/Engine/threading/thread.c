#include "Thread.h"
#include <Platform.h>
#include <stdlib.h>
#include <string.h> // memset


#ifdef LT_WINDOWS
#define PLATFORM_THREAD_SIZE 8 * 2
#define LOCK_SIZE 40
#else
#define PLATFORM_THREAD_SIZE 8 * 3
#define LOCK_SIZE 32
#endif

static uint64 threadIDCount = 0;

void ConstructThread(Thread* thread, const void* platformObj, const uint16 size, const char* name) {
  Thread tmp = {
    .name = name,
    .ID = threadIDCount++,
  };

  // Reserve heap mem and copy to heap
  memcpy(thread, &tmp.ID, sizeof(uint64) * 2);
  memcpy(thread->reserved, platformObj, size);

  thread->isValid = LT_TRUE;
}

Thread* LT_Thread_Create(ThreadFuncWrapper func, void* data, const char* name, ThreadLock* lock) {
    return PlatformThreadCreate(func, data, name, lock);
}

void LT_Thread_Join(const Thread* thread) {
  PlatformThreadJoin(thread);
}

void LT_Thread_Sleep(const Thread* thread, const uint64 miliseconds) {
  PlatformThreadSleep(thread, miliseconds);
}

void LT_Thread_Exit(const int32 exit_code) {
  PlatformThreadExit(exit_code);
}

void LT_Thread_ExitCode(Thread* thread) {
  PlatformThreadGetExitCode(thread);
}

void LT_Thread_SetLock(Thread* thread, ThreadLock* lock) {
  thread->lock = lock;
}

void LT_Thread_Destroy(Thread* thread) {
  PlatformThreadDestroy(thread);
  memset(thread->reserved, 0, PLATFORM_THREAD_SIZE);
  thread->isValid = LT_FALSE;
}

ThreadLock* LT_ThreadLock_Create() {
  return PlatformThreadLockCreate();
}

void LT_ThreadLock_Lock(ThreadLock* lock) {
  PlatformThreadLockLock(lock);
}

void LT_ThreadLock_Unlock(ThreadLock* lock) {
  PlatformThreadLockUnock(lock);
}

void LT_ThreadLock_Destroy(ThreadLock* lock) {
  PlatformThreadLockDestroy(lock);
}
