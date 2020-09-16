#include "Linux.h"

// Windows
Window window;
static bool shouldClose = LT_FALSE;


#ifndef LT_NO_MAIN //used for running tests
int main(int32 argc, const char **argv) {
  return 0;
}
#endif

LoadProc InitOpenGL(void) {
  return NULL;
}
SwapBuffersFunc GetPlatformSwapBuffer(void) {
  return NULL;
}

void PlatformInitInput(int32* in_keyStates) {}
uint8 PlatformGetKeyState(int32 keyState) {
  return 0;
}

void* PlatformLoadSharedLib(const char* name) {}
void* PlatformGetProc(const void* in_lib, const char* in_name) {}

Thread* PlatformThreadCreate(const Thread *thread, ThreadFuncWrapper funcWrapper) {
  return NULL;
}
void PlatformThreadStart(const Thread *thread) {}
void PlatformGetCurrent(const Thread *thread) {}
void PlatformThreadJoin(const Thread* thread) {}
void PlatformThreadSleep(const Thread* thread, const uint64 miliseconds) {}
void PlatformThreadExit(const int16 exit_code) {}
void PlatformThreadGetExitCode(Thread* thread) {}
void PlatformThreadDestroy(Thread* thread) {}

ThreadLock* PlatformThreadLockCreate() {
  return NULL;
}
void PlatformThreadLockLock(ThreadLock* lock) {}
void PlatformThreadLockUnock(ThreadLock* lock) {}
void PlatformThreadLockDestroy(ThreadLock* lock) {}