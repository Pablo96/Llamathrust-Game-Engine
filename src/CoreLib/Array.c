#include <CoreLib/Array.h>
#include <stdlib.h>
#include <string.h> //memcpy
#include <log.h>

Array LT_ArrayCreate(uint64 size, uint64 typeSize) {
    Array array = {
        .size = size,
        .typeSize = typeSize
    };

    array.data = malloc(size);
    return array;
}

Array LT_ArrayStackCreate(void* dataBuffer, uint64 size, uint64 typeSize) {
    Array array = {
        .data = dataBuffer,
        .size = size,
        .typeSize = typeSize
    };

    return array;
}

uint64 LT_ArrayCount(const Array* array) {
    return array->size / array->typeSize;
}

void LT_ArrayDestroy(Array* array) {
    free(array->data);
    array->data = NULL;
}

void* LT_ArrayGetElement(const Array* array, const uint64 index) {
    const uint64 actual_index = index * array->typeSize;
    LT_ASSERT(actual_index <= (array->size - array->typeSize), "Index out or range", 75);
    return (byte*) array->data + actual_index;
}

void LT_ArraySetElement(const Array* array, const uint64 index, void* element) {
    const uint64 actual_index = index * array->typeSize;
    
    LT_ASSERT(actual_index <= (array->size - array->typeSize), "Index out or range", 75);

    memcpy((byte*) array->data + actual_index, element, array->typeSize);
}
