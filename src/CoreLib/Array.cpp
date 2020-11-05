#include <CoreLib/Array.hpp>
#include <ErrorCodes.hpp>
#include <log.h>
#include <stdlib.h>
#include <string> //memcpy

Array LT_ArrayCreate(uint64 count, uint64 typeSize) {
  Array array = {
      .size = count * typeSize,
      .typeSize = typeSize,
  };

  array.data = malloc(count * typeSize);
  memset(array.data, 0, array.size);
  return array;
}

Array LT_ArrayStackCreate(void *dataBuffer, uint64 count, uint64 typeSize) {
  Array array = {
      .data = dataBuffer,
      .size = count * typeSize,
      .typeSize = typeSize
  };

  return array;
}

uint64 LT_ArrayCount(const Array *array) {
  return array->size / array->typeSize;
}

void LT_ArrayDestroy(Array *array) {
  free(array->data);
  array->data = NULL;
}

void *LT_ArrayGetElement(const Array *array, const uint64 index) {
  const uint64 actual_index = index * array->typeSize;
  LT_ASSERT(actual_index <= (array->size - array->typeSize),
            "Index out or range", ERROR_INDEX_OUT_OF_BOUNDS);
  return (const byte *)array->data + actual_index;
}

void LT_ArraySetElement(const Array *array, const uint64 index, void *element) {
  const uint64 actual_index = index * array->typeSize;

  LT_ASSERT(actual_index <= (array->size - array->typeSize),
            "Index out or range", ERROR_INDEX_OUT_OF_BOUNDS)

  memcpy((const byte *)array->data + actual_index, element, array->typeSize);
}
