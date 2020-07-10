#include "ThreadPool.h"
#include <CoreLib/Array.h>
#include <CoreLib/Queue.h>

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
  byte padding;
  Task* task,
} Worker;

static Worker* LT_WorkerCreate(void) {
  // TODO: Implement Worker constructor
}

static void LT_WorkerShutdown(Worker* worker) {
  // TODO: Implement Worker constructor
}

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
 * @field process:
 *	@type bool
 *	@brief if true the threads can take more tasks else they will stop fetching.
 **/
static struct ThreadPool {
  Queue tasks;
  Array threads;
  uint32 active_count;
  volatile bool process;
} Pool;

void LT_ThreadPoolInitialize(const uint32 min_threads, const uint32 max_threads, const uint64 max_tasks) {
  Pool.tasks = LT_QueueCreate(sizeof(Task) * max_tasks, sizeof(Task));
  Pool.threads = LT_ArrayCreate(sizeof(Worker*), max_threads);

  // Initialize the min number of threads
  for(int i = 0; i < min_threads; i++)
 	{
 		LT_ArraySetElement(&Pool.threads, i, LT_WorkerCreate());
 		Worker* worker =(Worker*) LT_ArraySetElement(&Pool.threads, i);
    // NOTE: See if is a good idea to add the thread to the array after initialized
    // or should be alloc and then started after being inserted.
    //LT_WorkerBegin(worker);

		if(worker->running)
			Pool.active_count++;
	}
}

void LT_ThreadPoolShutdown(void) {
  Pool.process = LT_FALSE;
  
  // Stop threads
  uint64 thread_count = LT_ArrayCount(&Pool.threads);
  for (uint64 i = 0; i < thread_count, i++) {
    Worker* worker = (Worker*) LT_ArrayGetElement(&Pool.threads, i);
    LT_WorkerShutdown(worker);
  }

  // Clear the arrays/queues
  LT_ArrayDestroy(&Pool.threads);
  LT_ArrayDestroy((Array*) &Pool.tasks);
}

void LT_ThreadPoolAddTask(ThreadFuncWrapper task, void* data) {
  Task task = {
    .task = task,
    .data = data
  };

  LT_QueuePush(&Pool.tasks, &task);
}
