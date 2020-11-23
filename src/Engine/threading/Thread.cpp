#include "Thread.hpp"
#include <Platform.hpp>
#include <ErrorCodes.hpp>
#include <log.hpp>
#include <cstring>

namespace LT {
// ID 0 is reserved for current thread
static uint64 threadIDCount = 1;

Thread::Thread(ThreadFuncWrapper func, void *in_data, const char *in_name,
               ThreadLock *in_lock)
    : lock(in_lock), ID(threadIDCount++), name(in_name), data(in_data),
      exitCode(0), isValid(true) {
  Platform::ThreadCreate(this, func);
}

void Thread::Start() { Platform::ThreadStart(this); }

Thread *Thread::GetCurrent() {
  Thread *tmp = new Thread(nullptr, nullptr, nullptr, nullptr);
  Platform::GetCurrent(tmp);
  return tmp;
}

void Thread::Join(Thread *thread_to_join) {
  Platform::ThreadJoin(thread_to_join);
}

void Thread::Sleep(const uint64 miliseconds) {
  Platform::ThreadSleep(this, miliseconds);
}

void Thread::Exit(const int32 exit_code) { Platform::ThreadExit(exit_code); }

uint64 Thread::CaptureExitCode() {
  Platform::ThreadGetExitCode(this);
  return this->exitCode;
}

void Thread::Destroy() {
  Platform::ThreadDestroy(this);
  std::memset((void *)this->reserved, 0, PLATFORM_THREAD_SIZE);
  this->isValid = false;
}

const char *Thread::GetName() { return this->name; }

void Thread::Lock() {
  LT_ASSERT(this->lock != nullptr, "Thread does not have lock",
            ERROR_THREADING_NO_LOCK)
  this->lock->Lock();
}

void Thread::Unlock() {
  LT_ASSERT(this->lock != nullptr, "Thread does not have lock",
            ERROR_THREADING_NO_LOCK)
  this->lock->Unlock();
}

ThreadLock::ThreadLock() {
  memcpy((void *)this->reserved, Platform::ThreadLockCreate(),
         sizeof(ThreadLock));
}

ThreadLock::~ThreadLock(void) { Platform::ThreadLockDestroy(this); }

void ThreadLock::Lock() { Platform::ThreadLockLock(this); }

void ThreadLock::Unlock() { Platform::ThreadLockUnock(this); }
} // namespace LT
