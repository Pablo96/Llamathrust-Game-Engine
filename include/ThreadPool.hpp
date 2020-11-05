/**
 * @file ThreadPool.h
 * @author Pablo Narvaja
 * @brief This file contains the api for a task based pool of threads.
 **/
#pragma once
#include <Common.hpp>
#include <Thread.hpp>

#define LT_THREAD_CREATETASK(TaskName) void TaskName(void* data)

void LT_ThreadPoolInitialize(const uint32 threads, const uint64 max_tasks);

void LT_ThreadPoolShutdown(void);

void LT_ThreadPoolAddTask(ThreadFuncWrapper taskFunc, void* data);
