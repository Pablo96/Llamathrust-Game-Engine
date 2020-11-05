/**
 * @file ThreadPool.h
 * @author Pablo Narvaja
 * @brief This file contains the api for a task based pool of threads.
 **/
#pragma once
#include <Common.hpp>
#include <Thread.hpp>

#define LT_THREAD_CREATETASK(TaskName) void TaskName(void* data)

namespace LT {
	void ThreadPoolInitialize(const uint32 threads, const uint64 max_tasks);

	void ThreadPoolShutdown(void);

	void ThreadPoolAddTask(ThreadFuncWrapper taskFunc, void* data);
}