#include <Common.hpp>
#include <cstring>

#include "test_framwork_common.hpp"

static TestNode *test_list = nullptr;
static uint64 test_count = 0;

static void CreateNode(TestNode *in_mem, func_proto func, const char *testName,
                       PROC_RETURN_T expected_code) {
  TestNode test;
  test.func = func;
  test.name = testName;
  test.next = nullptr;
  test.expected_code = expected_code;

  std::memcpy(in_mem, &test, sizeof(TestNode));
  test_count++;
}

uint64 GetTestCount() { return test_count; }

void NodeListPrepare(TestNode *list) {
  TestNode *current = test_list;
  for (uint64 i = 0; current != nullptr; i++) {
    std::memcpy(&list[i], current, sizeof(TestNode));
    current = current->next;
  }
}

void __TestAdd(func_proto func, const char *testName,
               PROC_RETURN_T expected_code) {
  TestNode *mem;
  if (test_list != nullptr) {
    TestNode *current = test_list;
    while (current->next != nullptr) {
      current = current->next;
    }

    mem = current->next = new TestNode();
  } else {
    mem = test_list = new TestNode();
  }

  CreateNode(mem, func, testName, expected_code);
}
