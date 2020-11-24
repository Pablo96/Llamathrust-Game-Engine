#pragma once
#include <Common.hpp>

#include "test_framework.hpp"

struct TestNode {
  func_proto func;
  const char *name;
  TestNode *next;
  PROC_RETURN_T expected_code;

  TestNode() {}
};

uint64 GetTestCount();

void NodeListPrepare(TestNode *list);
