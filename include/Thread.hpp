/**
 * @file thread.h
 * @author Pablo Narvaja
 * @brief Here is everything related to threading
 **/
#pragma once
#include <Common.hpp>

namespace LT {
struct Platform;
#ifdef LT_WINDOWS
#define PLATFORM_THREAD_SIZE 8 * 2
#define LOCK_SIZE 40
typedef uint64 (*ThreadFuncWrapper)(void *);
#else
#define PLATFORM_THREAD_SIZE sizeof(uint32)
#define LOCK_SIZE 40
typedef void *(*ThreadFuncWrapper)(void *);
#endif

#ifdef LT_DEBUG
#define ASSERT_RESERVED_SIZE(size)                                             \
  if (size > PLATFORM_THREAD_SIZE) {                                           \
    log_fatal("Size of platform object(%u) is greater than MAX_SIZE(%u)",      \
              size, PLATFORM_THREAD_SIZE);                                     \
    exit(36);                                                                  \
  }
#else
#define ASSERT_RESERVED_SIZE(size)
#endif

class ThreadLock {
  byte reserved[LOCK_SIZE];

public:
  ThreadLock(void);
  ~ThreadLock(void);
  void Lock();
  void Unlock();
};

#undef LOCK_SIZE

/**
 * @struct Thread
 * @brief represents a thread within the engine.
 * @note Called by the platform layer only!!
 * @field reserved:
 *	@type byte pointer
 *	@brief reserved for platform representation.
 * @field lock:
 *	@type ThreadLock pointer
 *	@brief OPTIONAL: lock for this thread to use.
 * @field ID
 *  @type const uint64
 *  @brief the ID of this thread.
 * @field name:
 *	@type const char pointer
 *	@brief OPTIONAL: name for the thread.
 * @field data:
 *	@type void pointer
 *	@brief OPTIONAL: user data that this thread holds.
 * @field exitCode
 *  @type int32
 *  @brief the exitCode of the thread
 * @field isValid
 *  @type bool
 *  @brief true when the thread has not been shutdown.
 **/
class Thread {
  friend Platform;

protected:
  byte reserved[PLATFORM_THREAD_SIZE];
  ThreadLock *lock;
  const uint64 ID;
  const char *name;
  void *data;
  int64 exitCode;
  bool isValid;

public:
  /**
   * @func LT_Thread_Create
   * @brief Creates a suspended thread.
   * @param func:
   *	@type ThreadFuncWrapper
   *	@brief function to execute on the thread.
   * @param data:
   *	@type void pointer
   *	@brief data that this thread holds.
   * @param name
   *  @type const char pointer
   *  @brief OPTIONAL: name of the thread.
   * @param lock
   *  @type ThreadLock pointer
   *  @brief OPTIONAL: lock for this thread to use.
   * @return Thread pointer
   *  @brief reference to the thread created.
   **/
  Thread(ThreadFuncWrapper func, void *data, const char *name,
         ThreadLock *lock);

  /**
   * @func LT_Thread_Create
   * @brief start the thread.
   * @note should be called once.
   * @return void
   **/
  void Start();

  static Thread *GetCurrent();

  /**
   * @func LT_Thread_Join
   * @brief waits for this thread to finish.
   * @return void
   **/
  void Join(Thread *thread_to_join);

  /**
   * @func LT_ThreadExitCode
   * @brief set the exit code of the thread if any.
   * @return int32
   * @brief the exit code
   **/
  uint64 CaptureExitCode();

  /**
   * @func LT_ThreadExit
   * @brief Exits from current thread with the given code.
   * @param exit_code:
   *	@type const int32
   *	@brief exit code of the thread.
   * @return void
   **/
  LT_NORETURN static void Exit(const int32 exit_code);

  /**
   * @func LT_Thread_Sleep
   * @brief Sleep the thread the specified ammount of time.
   * @note if miliseconds is 0 then it dont go to sleep at all.
   * @param millisecons:
   *	@type uint64
   *	@brief the amount of millisecond to wait.
   * @return void
   **/
  void Sleep(const uint64 miliseconds);

  /**
   * @func LT_ThreadDestroy
   * @brief invalidates this thread.
   **/
  void Destroy();

  const char *GetName();

  void Lock();

  void Unlock();
};

#undef MAX_RESERVED_SIZE
} // namespace LT
