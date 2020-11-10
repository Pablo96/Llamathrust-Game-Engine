/*
 * Copyright (c) 2017 rxi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <Common.hpp>

#ifdef LT_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
static PCRITICAL_SECTION CriticalSection;
#elif defined(LT_LINUX)
#include <pthread.h>
static pthread_mutex_t mutex;
#endif

#include "log.hpp"
static unsigned char init = !0;

static struct {
  void *udata;
  FILE *fp;
  int level;
  int quiet;
} L;


static const char *level_names[] = {
  "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {
  "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif

void log_set_udata(void *udata) {
  L.udata = udata;
}

void log_set_fp(FILE *fp) {
  L.fp = fp;
}


void log_set_level(int level) {
  L.level = level;
}


void log_set_quiet(int enable) {
  L.quiet = enable ? 1 : 0;
}


void log_log(int level, const char *file, int line, const char *fmt, ...) {
  if (level < L.level) {
    return;
  }

#ifdef LT_WINDOWS
  if (init) {
    CriticalSection = reinterpret_cast<CRITICAL_SECTION*>(malloc(sizeof(CRITICAL_SECTION)));
    // Initialize the critical section one time only.
    InitializeCriticalSectionAndSpinCount(CriticalSection, 0x00000400);
    init = 0;
  }
  /* Acquire lock */
  // Request ownership of the critical section.
  EnterCriticalSection(CriticalSection);
#elif defined(LT_LINUX)
  if (init) {
    pthread_mutex_init(&mutex, NULL);
    init = 0;
  }
  /* Acquire lock */
  // Request ownership of the mutex.
  pthread_mutex_lock(&mutex);
#endif

  /* Get current time */
  time_t t = time(NULL);
  struct tm *lt = localtime(&t);

  /* Log to stderr */
  if (!L.quiet) {
    va_list args;
    char buf[16];
    uint64 size = strftime(buf, sizeof(buf), "%H:%M:%S", lt);
    buf[size] = '\0';
#ifdef LOG_USE_COLOR
    fprintf(
      stderr, "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
      buf, level_colors[level], level_names[level], file, line);
#else
    if (level == LOG_TEST)
      fprintf(stderr, "%s %s: ", buf, file);
    else if (level == LOG_TEST_NFUNC)
      fprintf(stderr, "%s ", buf);
    else
      fprintf(stderr, "%s %-5s %s:%d: ", buf, level_names[level], file, line);
#endif
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    fflush(stderr);
  }

  /* Log to file */
  if (L.fp) {
    va_list args;
    char buf[32];
    buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
    fprintf(L.fp, "%s %-5s %s:%d: ", buf, level_names[level], file, line);
    va_start(args, fmt);
    vfprintf(L.fp, fmt, args);
    va_end(args);
    fprintf(L.fp, "\n");
    fflush(L.fp);
  }

  /* Release lock */
#ifdef LT_WINDOWS
  LeaveCriticalSection(CriticalSection);
#elif defined(LT_LINUX)
  pthread_mutex_unlock(&mutex);
#endif
}
