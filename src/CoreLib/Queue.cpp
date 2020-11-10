#include <CoreLib/Queue.hpp>

namespace LT {
    Queue::Queue(const uint64 in_count, const uint64 elementSize) : Array(count, elementSize),
        last_element_index(0), first_element_index(0), count(in_count), isEmpty(true)
    {}

    void* Queue::Pop() {
        void* element = GetElement(first_element_index);
        isEmpty = first_element_index > last_element_index;
        if (!isEmpty) {
            first_element_index++;
            first_element_index = first_element_index > count
                ? 0
                : first_element_index;
        }
        return element;
    }

    void* Queue::GetFirst() {
        return GetElement(first_element_index);
    }

    void Queue::Push(void* element) {
        if (isEmpty) {
            last_element_index = first_element_index;
            isEmpty = false;
        }
        else
            last_element_index++;

        last_element_index = last_element_index > count ?
            0 : last_element_index;
        SetElement(last_element_index, element);
    }

    bool Queue::IsEmpty()
    {
        return this->isEmpty;
    }

}