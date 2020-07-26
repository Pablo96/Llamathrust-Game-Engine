#include "test_framework.h"
#include <stdlib.h>
#include <string.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <malloc.h>

typedef struct _TestNode {
  int (*func)(void *);
  const char *name;
  struct _TestNode *next;
  const int expected_code;
} TestNode;

static TestNode *test_list = NULL;
static uint64 test_count = 0;

static void CreateNode(TestNode *in_mem, unsigned long (*func)(void *),
                       const char *testName, const int expected_code) {
  TestNode test = {.func = func,
                   .name = testName,
                   .expected_code = expected_code,
                   .next = NULL};
  memcpy(in_mem, &test, sizeof(TestNode));
  test_count++;
}

void __TestAdd(unsigned long (*func)(void *), const char *testName,
               const int expected_code) {
  TestNode *mem;
  if (test_list != NULL) {
    TestNode *current = test_list;
    while (current->next != NULL) {
      current = current->next;
    }

    mem = current->next = (TestNode *)malloc(sizeof(TestNode));
  } else {
    mem = test_list = (TestNode *)malloc(sizeof(TestNode));
  }

  CreateNode(mem, func, testName, expected_code);
}

static void Prepare(TestNode *list) {
  TestNode *current = test_list;
  for (uint64 i = 0; current != NULL; i++) {
    memcpy(&list[i], current, sizeof(TestNode));
    current = current->next;
  }
}

int LT_TestRun() {
  log_test_nfunc("Preparing %u Tests", test_count);

  HANDLE *threads = malloc(sizeof(HANDLE) * test_count);
  TestNode *list = malloc(sizeof(TestNode) * test_count);

  Prepare(list);

  log_test_nfunc("Spawning threads");
  // Create a thread for every test
  for (uint64 i = 0; i < test_count; i++) {
    threads[i] =
        CreateThread(NULL, 0, list[i].func, NULL, CREATE_SUSPENDED, NULL);
  }

  uint64 test_failed = 0;
  // Execute tests
  for (uint64 i = 0; i < test_count; i++) {
    const char *name = list[i].name;
    log_test_nfunc("Running test %s", name);

    HANDLE thread = threads[i];

    ResumeThread(thread);

    // Wait for test to finish
    WaitForSingleObject(thread, INFINITE);

    DWORD exitCode;
    GetExitCodeThread(thread, &exitCode);

    bool condition = exitCode == list[i].expected_code;
    const char *msg = (condition) ? "SUCCESS" : "FAILED";
    if (!condition) {
      test_failed++;
    }

    /* Get current time */
    log_test_nfunc("'%s' %s", list[i].name, msg);
  }
  log_test_nfunc("TOTAL: %u, \tSUCCEDED: %u, \tFAILED: %u", test_count,
                 test_count - test_failed, test_failed);

  free(threads);
  free(list);
  return test_failed;
}
