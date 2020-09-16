#include "Linux.h"
#include <dlfcn.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <log.h>
#include <stdnoreturn.h>
#include "Thread.h"
#include "ErrorCodes.h"

// Windows
Window window;
static bool shouldClose = LT_FALSE;

noreturn void LinuxHandleError(int32 in_exitCode);

#ifndef LT_NO_MAIN //used for running tests
int main(int32 argc, const char **argv) {
  return 0;
}
#endif

//-------------------------------------------
// Graphics
//-------------------------------------------

LoadProc InitOpenGL(void) {
  return NULL;
}
SwapBuffersFunc GetPlatformSwapBuffer(void) {
  return NULL;
}

//-------------------------------------------
// Input
//-------------------------------------------

void PlatformInitInput(int32* in_keyStates) {}
uint8 PlatformGetKeyState(int32 keyState) {
  return 0;
}

//-------------------------------------------
// Shared libs
//-------------------------------------------

void* PlatformLoadSharedLib(const char* name) {
  return dlopen(name, RTLD_LAZY);
}

void* PlatformGetProc(const void* in_lib, const char* in_name) {
  return dlsym(in_lib, in_name);
}

//-------------------------------------------
// Threads
//-------------------------------------------

Thread *PlatformThreadCreate(const Thread *thread, ThreadFuncWrapper funcWrapper) {
  pthread_t *threadID;
  int32 result = pthread_create(
          threadID,
          NULL,
          funcWrapper,
          &threadID);


  if (result != 0) {
    log_error("Failed to create thread.");
    LinuxHandleError(ERROR_PLATFORM_THREAD_CREATE)
  }

  ThreadLinux nixThread = { .id = threadID };
  memcpy(thread->reserved, &nixThread, sizeof(ThreadLinux));
  return thread;
}

void PlatformThreadStart(const Thread *thread) {
  //Does nothing
}

void PlatformGetCurrent(const Thread *thread) {
  pthread_t threadID = pthread_self();
  ThreadLinux nixThd = {.id = threadID };
  memcpy(thread->reserved, &nixThd, sizeof(ThreadLinux));
}

void PlatformThreadJoin(const Thread* thread) {
  pthread_join(thread->reserved, &thread->exitCode);
}

void PlatformThreadSleep(const Thread* thread, const uint64 miliseconds) {
  struct timespec ts;
  int res;

  if (miliseconds < 0)
    return;

  ts.tv_sec = miliseconds / 1000;
  ts.tv_nsec = (miliseconds % 1000) * 1000000;

  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);
}

void PlatformThreadExit(const int16 exit_code) {
  void *exitCode = (void*) exit_code;
  pthread_exit(exitCode);
}

void PlatformThreadGetExitCode(Thread* thread) {
  if (!pthread_tryjoin_np(thread->reserved, &thread->exitCode)) {
    log_error("Thread still active");
  } else {
    thread->isValid = LT_FALSE;
  }
}

void PlatformThreadDestroy(Thread* thread) {
  thread->isValid = LT_FALSE;
}

//-------------------------------------------
// Threading Locks
//-------------------------------------------

ThreadLock* PlatformThreadLockCreate() {
  pthread_mutex_t* lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(lock, NULL);
  return (ThreadLock*)lock;
}

void PlatformThreadLockLock(ThreadLock* lock) {
  pthread_mutex_lock(lock);
}

void PlatformThreadLockUnock(ThreadLock* lock) {
  pthread_mutex_unlock(lock);
}

void PlatformThreadLockDestroy(ThreadLock* lock) {
  pthread_mutex_destroy(lock);
}

noreturn void LinuxHandleError(int32 in_exitCode) {
  exit(in_exitCode);
}
