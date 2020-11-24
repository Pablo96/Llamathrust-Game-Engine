#include <stdlib.h>
#include <string.h>
#define WIN32_LEAN_AND_MEAN
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>
#include <malloc.h>

#include "test_framework.hpp"
#include "test_framwork_common.hpp"

uint64 LT_TestRun() {
  log_test_nfunc("Preparing %u Tests", test_count);

  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    const char log_msg[] = "WSAStartup failed with error: %d";
    log_error(log_msg, iResult);
  }

  HANDLE *threads = (HANDLE *)malloc(sizeof(HANDLE) * test_count);
  TestNode *list = (TestNode *)malloc(sizeof(TestNode) * test_count);

  Prepare(list);

  log_test_nfunc("Spawning threads");
  // Create a thread for every test
  for (uint64 i = 0; i < test_count; i++) {
    threads[i] = CreateThread(
        nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(list[i].func),
        nullptr, CREATE_SUSPENDED, nullptr);
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
  WSACleanup();

  return test_failed;
}
