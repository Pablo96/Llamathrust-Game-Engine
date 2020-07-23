#include "ThreadPool.h"
#include <CoreLib/Array.h>
#include <CoreLib/Queue.h>
#include <string.h> //memcpy
#include <ErrorCodes.h>
#include <log.h>

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
 * @field active_count:
 *	@type uint32
 *	@brief count of active threads:.
 * @field isProcessing:
 *	@type bool
 *	@brief if true the threads can take more tasks else they will stop fetching.
 **/
static struct ThreadPool {
  Queue tasks;
  Array threads;
  ThreadLock *lock;
  uint32 active_count;
  volatile bool isProcessing;
} *Pool;


/**
 * @func WorkerProc
 * @brief this is the worker procedure
 *        ran by every worker to do tasks.
 * @param _worker:
 *	@type void pointer
 *	@brief pointer to worker structure.
 **/
static void WorkerProc(void* _worker);

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
  void* data;
  ThreadFuncWrapper task;
} Task;


static Task* LT_ThreadPool_GetTask();

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
 * @field padding:
 *	@type byte
 *	@brief padding for data allignment
 * @field task:
 *	@type Task pointer
 *	@brief task we took from the pool.
 **/
typedef struct _Worker {
  Thread base;
  bool running;
  Task* task;
} Worker;

static void LT_WorkerCreate(Worker* worker, ThreadLock* lock) {
  worker->running = LT_TRUE;
  worker->task = NULL;

  LT_Thread_Create(worker, WorkerProc, NULL, "worker", lock);
}

static void LT_WorkerShutdown(Worker* worker) {
  LT_ThreadLock_Lock(Pool->lock);

  memset(&worker->base.isValid, 0, sizeof(Worker) - offsetof(Thread, isValid));
  LT_Thread_Join(worker);

  LT_Thread_ExitCode(worker); 
  LT_Thread_Destroy(worker);
  
  LT_ThreadLock_Unlock(Pool->lock);
}

void WorkerProc(void* _worker) {
  Worker* this = _worker;

  while(this->running)
  {
    LT_Thread_Sleep(this, 1);
    if (this->task != NULL) {
      void* data = this->task->data;
      ThreadFuncWrapper task = this->task->task;

      task(data);
      
      this->task = NULL;
    }
    
    // If we're finished with our task, grab a new one.
	if(this->task == NULL && Pool->isProcessing == LT_TRUE) {
        LT_ThreadLock_Lock(this->base.lock);
		this->task = LT_ThreadPool_GetTask();
        LT_ThreadLock_Unlock(this->base.lock);
	}
  }

  LT_Thread_Exit(0);
}

void LT_ThreadPoolInitialize(const uint32 threads_count, const uint64 max_tasks) {
  ThreadLock* lock = LT_ThreadLock_Create();
  struct ThreadPool pool = {
    .tasks = LT_QueueCreate(sizeof(Task) * max_tasks, sizeof(Task)),
      .threads = LT_ArrayCreate(threads_count, sizeof(Worker)),
    .lock = lock,
    .active_count = 0,
    .isProcessing = LT_FALSE
  };
  
  Pool = (struct ThreadPool*) malloc(sizeof(struct ThreadPool));
  memcpy(Pool, &pool, sizeof(struct ThreadPool));


  // Initialize the min number of threads
  for(uint32 i = 0; i < threads_count; i++) {
    Worker* worker =(Worker*) LT_ArrayGetElement(&Pool->threads, i);
    LT_WorkerCreate(worker, lock);
    LT_Thread_Start(worker);
    if(worker->running)
      Pool->active_count++;
  }
}

void LT_ThreadPoolShutdown() {
  Pool->isProcessing = LT_FALSE;
  
  // Stop threads
  uint64 thread_count = LT_ArrayCount(&Pool->threads);
  for (uint64 i = 0; i < thread_count; i++) {
    Worker* worker = (Worker*) LT_ArrayGetElement(&Pool->threads, i);
    LT_WorkerShutdown(worker);
  }

  // Clear the arrays/queues
  LT_QueueDestroy(&Pool->tasks);
  LT_ArrayDestroy(&Pool->threads);
}

void LT_ThreadPoolAddTask(ThreadFuncWrapper taskFunc, void* data) {
  LT_ASSERT(taskFunc != NULL, "Task function null not allowed", ERROR_NULL_ARG)
  LT_ThreadLock_Lock(Pool->lock);
  
  Task task = {
    .task = taskFunc,
    .data = data
  };

  LT_QueuePush(&Pool->tasks, &task);

  if (!Pool->isProcessing)
    Pool->isProcessing = LT_TRUE;

  LT_ThreadLock_Unlock(Pool->lock);
}


Task* LT_ThreadPool_GetTask() {
  Task* task = LT_QueuePop(&Pool->tasks);
  Pool->isProcessing = Pool->tasks.isEmpty;
  return task;
}