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

// ID 0 is reserved for current thread
static uint64 threadIDCount = 1;

Thread *LT_Thread_Create(Thread *this, ThreadFuncWrapper func, void *data,
                         const char *name, ThreadLock *lock) {
  if (this == NULL) {
    this = malloc(sizeof(Thread));
  }
  
  Thread tmp = {
      .lock = lock,
      .ID = threadIDCount++,
      .name = name,
      .data = data,
      .exitCode = 0,
      .isValid = LT_TRUE
  };

  
  memcpy(&this->lock, &tmp.lock, sizeof(Thread) - PLATFORM_THREAD_SIZE);
  return PlatformThreadCreate(this, func);
}

void LT_Thread_Start(Thread *thread) {
  PlatformThreadStart(thread);
}

Thread* LT_Thread_GetCurrent(Thread *this) {
  if (this == NULL) {
    this = malloc(sizeof(Thread));
  }
  
  Thread tmp = {
      .lock = NULL,
      .ID = 0,
      .name = NULL,
      .data = NULL,
      .exitCode = 0,
      .isValid = LT_TRUE
  };

  PlatformGetCurrent(&tmp);

  // Search for current thread
  memcpy(this, &tmp, sizeof(Thread));

  return this;
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
