#include "Thread.hpp"
#include <Platform.hpp>
#include <stdlib.h>
#include <string> // memset

namespace LT {
    // ID 0 is reserved for current thread
    static uint64 threadIDCount = 1;

    Thread::Thread(ThreadFuncWrapper func, void* in_data, const char* in_name, ThreadLock* in_lock)
        : lock(in_lock), ID(threadIDCount++), name(in_name), data(in_data), exitCode(0), isValid(true) {
        Platform::ThreadCreate(this, func);
    }

    void Thread::Start() { Platform::ThreadStart(this); }

    Thread* Thread::GetCurrent() {
        Thread* tmp = new Thread(nullptr, nullptr, nullptr, nullptr);
        Platform::GetCurrent(tmp);
        return tmp;
    }

    void Thread::Join() { Platform::ThreadJoin(this); }

    void Thread::Sleep(const uint64 miliseconds) {
        Platform::ThreadSleep(this, miliseconds);
    }

    void Thread::Exit(const int32 exit_code) { Platform::ThreadExit(exit_code); }

    void Thread::CaptureExitCode() { Platform::ThreadGetExitCode(this); }

    void Thread::Destroy() {
        Platform::ThreadDestroy(this);
        memset((void*)this->reserved, 0, PLATFORM_THREAD_SIZE);
        this->isValid = false;
    }

    ThreadLock::ThreadLock() {
        memcpy((void*)this->reserved, Platform::ThreadLockCreate(), sizeof(ThreadLock));
    }

    ThreadLock::~ThreadLock(void)
    {
        Platform::ThreadLockDestroy(this);
    }

    void ThreadLock::Lock() { Platform::ThreadLockLock(this); }

    void ThreadLock::Unlock() { Platform::ThreadLockUnock(this); }
}
