/**
 * @file ThreadPool.h
 * @author Pablo Narvaja
 * @brief This file contains the api for a task based pool of threads.
 **/
#pragma once
#include <Common.hpp>
#include <Thread.hpp>

#define LT_THREAD_CREATETASK(TaskName) void #TaskName(void* data)

#ifdef LT_WINDOWS
#define WORKER_PROC_SIGNATURE uint64 WorkerProc(void* _worker)
#else
#define WORKER_PROC_SIGNATURE void WorkerProc(void* _worker)
#endif

namespace LT {
	struct Task;

	struct ThreadPool {
		static void Initialize(const uint32 threads, const uint64 max_tasks);

		static void Shutdown(void);

		static void AddTask(ThreadFuncWrapper taskFunc, void* data);

	protected:
		static Task* GetTask();

		friend WORKER_PROC_SIGNATURE;
	};
}
