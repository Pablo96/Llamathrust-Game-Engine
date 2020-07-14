#pragma once
#include "test_framework.h"
#include <CoreLib/Array.h>
#include <CoreLib/Stack.h>
#include <CoreLib/String.h>

START_TEST(TestArray)
  Array array = LT_ArrayCreate(8 * 10, 8);
  
  uint64 count = LT_ArrayCount(&array);
  log_info("Count %u", count);
  
  for (uint64 i = 0; i < count; i++) {
    uint64 element = 9 * i;
    LT_ArraySetElement(&array, i, &element);
  }

  for (uint64 i = 0; i < count; i++) {
    uint64 element = *(uint64*) LT_ArrayGetElement(&array, i);
    log_info("number: %u", element);
  }

  LT_ArrayDestroy(&array);

  return TEST_SUCCESS;
END_TEST


START_TEST(TestArrayOutRange)
  Array array = LT_ArrayCreate(8 * 10, 8);
  uint64 count = LT_ArrayCount(&array);
  
  for (uint64 i = count; i < count + 5; i++) {
    uint64 element = 9 * i;
    LT_ArraySetElement(&array, i, &element);
  }

  LT_ArrayDestroy(&array);

  return TEST_SUCCESS;
END_TEST


START_TEST(TestStack)
  Stack stack = LT_StackCreate(8 * 10, 8);
  
  uint64 count = LT_ArrayCount(&stack);
  log_info("Reserved count %u", count);
  
  for (uint64 i = 0; i < count; i++) {
    uint64 element = 9 * i;
    LT_StackPush(&stack, &element);
    log_info("pushed number: %u", element);
  }

  uint64 top = *(uint64*) LT_StackTop(&stack);
  log_info("top number: %u", top);

  for (uint64 i = 0; i < count; i++) {
    uint64 element = *(uint64*) LT_StackPop(&stack);
    log_info("poped number: %u", element);
  }

  LT_StackDestroy(&stack);

  return TEST_SUCCESS;
END_TEST
