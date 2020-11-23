/**
 * Copyright (c) 2017 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <Common.hpp>

#define LOG_VERSION "0.1.0"

typedef void (*log_LockFn)(void *udata, int lock);

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL, LOG_TEST, LOG_TEST_NFUNC };

#ifdef LT_RELEASE
    #define log_trace(...)
    #define log_debug(...)
    #define log_info(...)
    #define log_warn(...)
#else
    #define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
    #define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
    #define log_info(...)  log_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
    #define log_warn(...)  log_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#endif
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)
#define log_test(...)  log_log(LOG_TEST,  __FUNCTION__, 0, __VA_ARGS__)
#define log_test_nfunc(...) log_log(LOG_TEST_NFUNC, 0, 0, __VA_ARGS__)

void log_set_udata(void *udata);
void log_set_fp(FILE *fp);
void log_set_level(int level);
void log_set_quiet(int enable);

void log_log(int level, const char *file, int line, const char *fmt, ...);
