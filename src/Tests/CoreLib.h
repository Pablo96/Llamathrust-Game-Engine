#pragma once
#include "test_framework.h"
#include <CoreLib/Array.h>
#include <CoreLib/Stack.h>
#include <CoreLib/Queue.h>
#include <CoreLib/String.h>

START_TEST(TestArray)
  uint64 count = 10;
  Array array = LT_ArrayCreate(count, 8);
  
  for (uint64 i = 0; i < count; i++) {
    uint64 element = 9 * i;
    LT_ArraySetElement(&array, i, &element);
  }

  for (uint64 i = 0; i < count; i++) {
    uint64 element = *(uint64*) LT_ArrayGetElement(&array, i);
    log_test("number: %u", element);
  }

  LT_ArrayDestroy(&array);

  return TEST_SUCCESS;
END_TEST


START_TEST(TestArrayOutRange)
  uint64 count = 10;
  Array array = LT_ArrayCreate(count, 8);
  
  for (uint64 i = count; i < count + 5; i++) {
    uint64 element = 9 * i;
    LT_ArraySetElement(&array, i, &element);
  }

  LT_ArrayDestroy(&array);

  return TEST_SUCCESS;
END_TEST

START_TEST(TestArrayCount)
  uint64 init_count = 10;
  Array array = LT_ArrayCreate(init_count, 8);
  
  uint64 count = LT_ArrayCount(&array);  
  log_test("Reserved count (%u): %u", init_count, count);

  LT_ArrayDestroy(&array);

  return TEST_ASSERT(count == init_count);
END_TEST

START_TEST(TestStack)
  uint64 count = 10;
  Stack stack = LT_StackCreate(count, 8);
  
  uint64 last_element = 0;
  for (uint64 i = 0; i < count; i++) {
    last_element = 9 * i;
    LT_StackPush(&stack, &last_element);
  }

  uint64 top = *(uint64*) LT_StackTop(&stack);
  log_test("Top (%u): %u",  last_element, top);
  
  LT_StackDestroy(&stack);

  return TEST_ASSERT(top == last_element);
END_TEST

START_TEST(TestQueue)
  uint64 count = 10;
  Queue queue = LT_QueueCreate(count, 8);
  
  uint64 last_element_pushed = 0;
  for (uint64 i = 0; i < count; i++) {
    last_element_pushed = 9 * i;
    LT_QueuePush(&queue, &last_element_pushed);
  }

  uint64 last_element_poped = 0;
  for (uint64 i = 0; i < count; i++) {
    last_element_poped = *(uint64*) LT_QueuePop(&queue);
  }

  LT_QueueDestroy(&queue);

  log_test(" Last_Push %u : Last_Pop %u", last_element_pushed, last_element_poped);
  return TEST_ASSERT(last_element_poped == last_element_pushed);
END_TEST
