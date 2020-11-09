#include "Thread.hpp"
#include <Platform.hpp>
#include <stdlib.h>
#include <string> // memset

namespace LT {
    // ID 0 is reserved for current thread
    static uint64 threadIDCount = 1;

    Thread::Thread(ThreadFuncWrapper func, void* in_data, const char* in_name, ThreadLock* in_lock)
        : ID(threadIDCount++), lock(in_lock), name(in_name), data(in_data), exitCode(0), isValid(true) {
        Platform::PlatformThreadCreate(this, func);
    }

    void Thread::Start() { Platform::PlatformThreadStart(this); }

    Thread* Thread::GetCurrent() {
        Thread* tmp = new Thread(nullptr, nullptr, nullptr, nullptr);
        Platform::PlatformGetCurrent(tmp);
        return tmp;
    }

    void Thread::Join(const Thread* thread) { Platform::PlatformThreadJoin(thread); }

    void Thread::Sleep(const uint64 miliseconds) {
        Platform::PlatformThreadSleep(this, miliseconds);
    }

    void Thread::Exit(const int32 exit_code) { Platform::PlatformThreadExit(exit_code); }

    void Thread::CaptureExitCode() { Platform::PlatformThreadGetExitCode(this); }

    void Thread::Destroy() {
        Platform::PlatformThreadDestroy(this);
        memset((void*)this->reserved, 0, PLATFORM_THREAD_SIZE);
        this->isValid = false;
    }

    ThreadLock::ThreadLock() {
        memcpy((void*)this->reserved, Platform::PlatformThreadLockCreate(), sizeof(ThreadLock));
    }

    ThreadLock::~ThreadLock(void)
    {
        Platform::PlatformThreadLockDestroy(this);
    }

    void ThreadLock::Lock() { Platform::PlatformThreadLockLock(this); }

    void ThreadLock::Unlock() { Platform::PlatformThreadLockUnock(this); }
}