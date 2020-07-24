#include <CoreLib/Queue.h>

Queue LT_QueueCreate(const uint64 count, const uint64 elementSize) {
    Queue queue = {
        .array = LT_ArrayCreate(count, elementSize),
        .last_element_index = 0,
        .first_element_index = 0,
        .count = count,
        .isEmpty = LT_TRUE
    };

    return queue;
}

void LT_QueueDestroy(Queue* queue) {
    LT_ArrayDestroy(queue);
}

void* LT_QueuePop(Queue* queue) {
    void* element = LT_ArrayGetElement(queue, queue->first_element_index);
    queue->isEmpty = queue->first_element_index > queue->last_element_index;
    if (!queue->isEmpty) {
        queue->first_element_index++;
        queue->first_element_index = queue->first_element_index > queue->count ? 0 : queue->first_element_index;
    }
    return element;
}

void* LT_QueueFirst(Queue* queue) {
    return LT_ArrayGetElement(queue, queue->first_element_index);
}

void LT_QueuePush(Queue* queue, void* element) {
    if (queue->isEmpty) {
        queue->last_element_index = queue->first_element_index;
        queue->isEmpty = LT_FALSE;
    }
    else
        queue->last_element_index++;

    queue->last_element_index = queue->last_element_index > queue->count ? 0 : queue->last_element_index;
    LT_ArraySetElement(queue, queue->last_element_index, element);
}
