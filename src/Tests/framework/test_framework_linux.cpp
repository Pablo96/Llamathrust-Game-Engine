#include <malloc.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include <stdexcept>

#include "test_framework.hpp"
#include "test_framwork_common.hpp"

PROC_RETURN_T thread_tester(void *test) {
  TestNode *testNode = reinterpret_cast<TestNode *>(test);
  PROC_RETURN_T exit_code;
  try {
    exit_code = testNode->func(nullptr);
  } catch (std::exception &e) {
    exit_code = e.what();
  }
  return exit_code;
}

uint64 LT_TestRun() {
  uint64 test_count = GetTestCount();
  log_test_nfunc("Preparing %u Tests", test_count);

  pthread_t *threads = new pthread_t[test_count];
  TestNode *list = new TestNode[test_count];

  NodeListPrepare(list);

  uint64 test_failed = 0;
  // Execute tests
  for (uint64 i = 0; i < test_count; i++) {
    const char *name = list[i].name;
    log_test_nfunc("Running test %s", name);

    pthread_create(&threads[i], nullptr, thread_tester, &list[i]);

    void *exitCode;
    pthread_join(threads[i], &exitCode);

    bool condition = exitCode == SET_EXPECTED_RESULT(list[i].expected_code);
    const char *msg = (condition) ? "SUCCESS" : "FAILED";
    if (!condition) {
      test_failed++;
    }

    /* Get current time */
    log_test_nfunc("'%s' %s", list[i].name, msg);
  }
  log_test_nfunc("TOTAL: %u, \tSUCCEDED: %u, \tFAILED: %u", test_count,
                 test_count - test_failed, test_failed);

  delete threads;
  delete list;

  return test_failed;
}
