#pragma once
#include "test_framework.hpp"
#include <CoreLib/Array.hpp>
#include <CoreLib/Queue.hpp>
#include <CoreLib/Stack.hpp>
#include <CoreLib/String.hpp>

START_TEST(TestArray)
uint64 count = 10;
LT::Array array = LT::Array(count, 8);

for (uint64 i = 0; i < count; i++) {
  uint64 element = 9 * i;
  array.SetElement(i, &element);
}

for (uint64 i = 0; i < count; i++) {
  uint64 element = *(uint64 *)array.GetElement(i);
  log_test("number: %u", element);
}

return TEST_SUCCESS;
END_TEST

START_TEST(TestArrayOutRange)
uint64 count = 10;
LT::Array array = LT::Array(count, 8);

for (uint64 i = count; i < count + 5; i++) {
  uint64 element = 9 * i;
  array.SetElement(i, &element);
}

return TEST_SUCCESS;
END_TEST

START_TEST(TestArrayCount)
uint64 init_count = 10;
LT::Array array = LT::Array(init_count, 8);

uint64 count = array.Count();
log_test("Reserved count (%u): %u", init_count, count);

return TEST_ASSERT(count == init_count);
END_TEST

START_TEST(TestStack)
uint64 count = 10;
LT::Stack stack = LT::Stack(count, 8);

uint64 last_element = 0;
for (uint64 i = 0; i < count; i++) {
  last_element = 9 * i;
  stack.Push(&last_element);
}

uint64 top = *(uint64 *)stack.Top();
log_test("Top (%u): %u", last_element, top);

return TEST_ASSERT(top == last_element);
END_TEST

START_TEST(TestQueue)
uint64 count = 10;
LT::Queue queue = LT::Queue(count, 8);

uint64 last_element_pushed = 0;
for (uint64 i = 0; i < count; i++) {
  last_element_pushed = 9 * i;
  queue.Push(&last_element_pushed);
}

uint64 last_element_poped = 0;
for (uint64 i = 0; i < count; i++) {
  last_element_poped = *(uint64 *)queue.Pop();
}

log_test(" Last_Push %u : Last_Pop %u", last_element_pushed,
         last_element_poped);
return TEST_ASSERT(last_element_poped == last_element_pushed);
END_TEST
