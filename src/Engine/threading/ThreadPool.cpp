#include "ThreadPool.hpp"
#include <CoreLib/Array.hpp>
#include <CoreLib/Queue.hpp>
#include <ErrorCodes.hpp>
#include <log.hpp>
#include <string.h> //memcpy

#ifdef LT_LINUX
// Include for offsetof
#include <stddef.h>
#endif
/**
 * @struct ThreadPool
 * @brief Thread pool abstraction.
 * @field tasks:
 *	@type Queue
 *	@brief queue of tasks.
 * @field threads:
 *	@type Thread pointer
 *	@brief threads array.
 * @field threadsCount:
 *	@type uint32
 *	@brief ammount of threads in the threads array.
 * @field isProcessing:
 *	@type bool
 *	@brief if true the threads can take more tasks else they will stop
 *fetching.
 **/
static struct ThreadPool {
  Queue tasks;
  Array threads;
  ThreadLock *lock;
  volatile bool isProcessing;
} * Pool;

/**
 * @func WorkerProc
 * @brief this is the worker procedure
 *        ran by every worker to do tasks.
 * @param _worker:
 *	@type void pointer
 *	@brief pointer to worker structure.
 **/
static void WorkerProc(void *_worker);

/**
 * @struct Task
 * @brief Task abstraction.
 * @field data:
 *	@type void pointer
 *	@brief data passed to the task function
 * @field task:
 *	@type ThreadFuncWrapper
 *	@brief the task function
 **/
typedef struct _Task {
  void *data;
  ThreadFuncWrapper task;
} Task;

static Task *LT_ThreadPool_GetTask();

/**
 * @struct Worker
 * @brief Thread in the pool that excecute tasks.
 * @field base:
 *	@type Thread
 *	@brief inherit thread capabilities.
 * @field running:
 *	@type bool
 *	@brief True once the thread initializes successfully,
 *          False if it fails or gets shut down.
 * @field task:
 *	@type Task pointer
 *	@brief task we took from the pool.
 **/
typedef struct _Worker {
  Thread base;
  bool running;
  Task *task;
} Worker;

static void LT_WorkerCreate(Worker *worker, ThreadLock *lock) {
  worker->running = true;
  worker->task = NULL;

  LT_Thread_Create(worker, WorkerProc, NULL, "worker", lock);
}

static void LT_WorkerShutdown(Worker *worker) {
  memset(&worker->running, 0, sizeof(Worker) - offsetof(Worker, running));

  // Wait for worker to finsish its task
  LT_Thread_Join(worker);

  // Get exit code
  LT_Thread_ExitCode(worker);

  // Destroy worker thread
  LT_Thread_Destroy(worker);
}

static void WorkerProc(void *_worker) {
  Worker *this = _worker;

  while (this->running) {
    if (this->task != NULL) {
      ThreadFuncWrapper task = this->task->task;
      if (task == NULL)
        continue;
      void *data = this->task->data;

      task(data);

      this->task = NULL;
    }

    if (Pool->isProcessing == true) {
      LT_ThreadLock_Lock(this->base.lock);
      this->task = LT_ThreadPool_GetTask();
      LT_ThreadLock_Unlock(this->base.lock);
    }
  }

  LT_Thread_Exit(0);
}

void LT_ThreadPoolInitialize(const uint32 threads_count,
                             const uint64 max_tasks) {
  ThreadLock *lock = LT_ThreadLock_Create();
  struct ThreadPool pool = {
      .tasks = LT_QueueCreate(sizeof(Task) * max_tasks, sizeof(Task)),
      .threads = LT_ArrayCreate(threads_count, sizeof(Worker)),
      .lock = lock,
      .isProcessing = false};

  Pool = (struct ThreadPool *)malloc(sizeof(struct ThreadPool));
  memcpy(Pool, &pool, sizeof(struct ThreadPool));

  // Spawn workers
  for (uint32 i = 0; i < threads_count; i++) {
    Worker *worker = LT_ArrayGetElement(&Pool->threads, i);
    LT_WorkerCreate(worker, lock);
    LT_Thread_Start(worker);
  }
}

void LT_ThreadPoolShutdown() {
  // Stop task retrieving
  Pool->isProcessing = false;

  // Stop threads
  uint32 threads_count = (uint32)LT_ArrayCount(&Pool->threads);
  for (uint32 i = 0; i < threads_count; i++) {
    Worker *worker = LT_ArrayGetElement(&Pool->threads, i);
    LT_WorkerShutdown(worker);
  }

  // free the pool object and its members
  LT_QueueDestroy(&Pool->tasks);
  LT_ArrayDestroy(&Pool->threads);
  LT_ThreadLock_Destroy(Pool->lock);
  free(Pool);
}

void LT_ThreadPoolAddTask(ThreadFuncWrapper taskFunc, void *data) {
  LT_ASSERT(taskFunc != NULL, "A task should be provided!", ERROR_NULL_ARG)

  Task task = {.task = taskFunc, .data = data};

  LT_QueuePush(&Pool->tasks, &task);
  Pool->isProcessing = true;
}

Task *LT_ThreadPool_GetTask() {
  Task *task = LT_QueuePop(&Pool->tasks);
  Pool->isProcessing = !Pool->tasks.isEmpty;
  return task;
}
