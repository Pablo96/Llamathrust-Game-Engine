#include <CoreLib/Stack.hpp>

namespace LT {
Stack::Stack(const uint64 size, const uint64 typeSize)
    : Array(size, typeSize), isEmpty(true) {}

void* Stack::Pop() {
  if (!this->elements_count)
    this->isEmpty = true;
  else
    this->elements_count--;
  return GetElement(elements_count);
}

void* Stack::Top() { return GetElement(elements_count - 1); }

void Stack::Push(void* element) {
  SetElement(elements_count, element);
  elements_count++;
  isEmpty = false;
}
}  // namespace LT
