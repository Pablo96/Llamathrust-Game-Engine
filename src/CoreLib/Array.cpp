#include <CoreLib/Array.hpp>
#include <ErrorCodes.hpp>
#include <log.hpp>
#include <stdlib.h>
#include <string> //memcpy

namespace LT {
    Array::Array(const uint64 count, const uint64 typeSize)
        : size(count * typeSize), typeSize(typeSize) {

        this->data = malloc(count * typeSize);
        memset(this->data, 0, this->size);
    }

    Array::Array(void* dataBuffer, uint64 count, uint64 typeSize) :
            data(dataBuffer),
            size(count * typeSize),
            typeSize(typeSize)
    {}

    Array::~Array() {
        free(this->data);
        this->data = NULL;
    }

    uint64 Array::Count()
    {
        return this->size / this->typeSize;
    }

    void* Array::GetElement(const uint64 index) {
        const uint64 actual_index = index * this->typeSize;
        LT_ASSERT(actual_index <= (this->size - this->typeSize),
            "Index out or range", ERROR_INDEX_OUT_OF_BOUNDS);
        return (void*)((const byte*)this->data + actual_index);
    }

    void Array::SetElement(const uint64 index, void* element) {
        const uint64 actual_index = index * this->typeSize;

        LT_ASSERT(actual_index <= (this->size - this->typeSize), "Index out or range", ERROR_INDEX_OUT_OF_BOUNDS);

        memcpy((void*) (reinterpret_cast<const byte*>(this->data) + actual_index), element, this->typeSize);
    }


    Array Array::operator=(const Array& array)
    {
        return Array(array);
    }
}