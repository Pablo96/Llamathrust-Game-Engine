#include <CoreLib/Array.hpp>
#include <ErrorCodes.hpp>
#include <log.hpp>
#include <stdlib.h>
#include <string> //memcpy

#ifdef LT_TEST_FRAMEWORK
#include <Thread.hpp>
#endif

namespace LT {
Array::Array(const uint64 in_count, const uint64 in_typeSize)
    : size(in_count * in_typeSize), typeSize(in_typeSize),
      elements_count(in_count) {

  this->data = malloc(elements_count * typeSize);
  memset(this->data, 0, this->size);
}

Array::Array(void *dataBuffer, uint64 in_count, uint64 in_typeSize)
    : data(dataBuffer), size(in_count * in_typeSize), typeSize(in_typeSize),
      elements_count(in_count) {}

Array::~Array() {
  free(this->data);
  this->data = nullptr;
}

uint64 Array::Count() { return this->elements_count; }

void *Array::GetElement(const uint64 index) {
  const uint64 actual_index = index * this->typeSize;
  LT_ASSERT(actual_index <= (this->size - this->typeSize), "Index out or range",
            ERROR_INDEX_OUT_OF_BOUNDS);
  return (void *)((const byte *)this->data + actual_index);
}

void Array::SetElement(const uint64 index, void *element) {
  const uint64 actual_index = index * this->typeSize;

  LT_ASSERT(actual_index <= (this->size - this->typeSize), "Index out of range",
            ERROR_INDEX_OUT_OF_BOUNDS);

  memcpy((void *)(reinterpret_cast<const byte *>(this->data) + actual_index),
         element, this->typeSize);
}

Array Array::operator=(const Array &array) { return Array(array); }
} // namespace LT
