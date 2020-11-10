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

namespace LT {
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
    static struct _ThreadPool {
        Queue tasks;
        Array threads;
        ThreadLock* lock;
        volatile bool isProcessing;
    } Pool;

    /**
     * @func WorkerProc
     * @brief this is the worker procedure
     *        ran by every worker to do tasks.
     * @param _worker:
     *	@type void pointer
     *	@brief pointer to worker structure.
     **/
    WORKER_PROC_SIGNATURE;


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
    struct Task {
        void* data;
        ThreadFuncWrapper task;

        Task(void* in_data, ThreadFuncWrapper in_task)
            : data(in_data), task(in_task) {}
    };


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
    struct Worker : public Thread {
        bool running;
        Task* task;

        Worker(ThreadLock* lock) : 
            Thread(WorkerProc, NULL, "worker", lock), running(true), task(nullptr) {}

        void Lock() {
            this->lock->Lock();
        }

        void Unlock() {
            this->lock->Unlock();
        }
    };

    static void LT_WorkerShutdown(Worker* worker) {
        worker->running = false;
        worker->task = nullptr;

        // Wait for worker to finsish its task
        worker->Join();

        // Get exit code
        worker->CaptureExitCode();

        // Destroy worker thread
        worker->Destroy();

        delete worker;
        worker = nullptr;
    }

    WORKER_PROC_SIGNATURE {
        Worker* worker = reinterpret_cast<Worker*>(_worker);

        while (worker->running) {
            if (worker->task != NULL) {
                ThreadFuncWrapper task = worker->task->task;
                if (task == NULL)
                    continue;
                void* data = worker->task->data;

                task(data);

                worker->task = NULL;
            }

            if (Pool.isProcessing == true) {
                worker->Lock();
                worker->task = ThreadPool::GetTask();
                worker->Unlock();
            }
        }

        Thread::Exit(0);
    }

    void ThreadPool::Initialize(const uint32 threads_count, const uint64 max_tasks) {
        ThreadLock* lock = new ThreadLock();

        Pool.tasks = Queue(sizeof(Task) * max_tasks, sizeof(Task));
        Pool.threads = Array(threads_count, sizeof(Worker));
        Pool.lock = lock;
        Pool.isProcessing = false;

        // Spawn workers
        for (uint32 i = 0; i < threads_count; i++) {
            Worker* worker = reinterpret_cast<Worker*>(Pool.threads.GetElement(i));
            new (worker) Worker(lock);
            worker->Start();
        }
    }

    void ThreadPool::Shutdown() {
        // Stop task retrieving
        Pool.isProcessing = false;

        // Stop threads
        uint32 threads_count = (uint32)Pool.threads.Count();
        for (uint32 i = 0; i < threads_count; i++) {
            Worker* worker = reinterpret_cast<Worker*>(Pool.threads.GetElement(i));
            LT_WorkerShutdown(worker);
        }

        delete Pool.lock;
    }

    void ThreadPool::AddTask(ThreadFuncWrapper taskFunc, void* data) {
        LT_ASSERT(taskFunc != NULL, "A task should be provided!", ERROR_NULL_ARG);

        Task task(data, taskFunc);

        Pool.tasks.Push(&task);
        Pool.isProcessing = true;
    }

    Task* ThreadPool::GetTask() {
        Task* task = reinterpret_cast<Task*>(Pool.tasks.Pop());
        Pool.isProcessing = !Pool.tasks.IsEmpty();
        return task;
    }
}
