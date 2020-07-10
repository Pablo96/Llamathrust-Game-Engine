/**
 * @file Queue.h
 * @author Pablo Narvaja
 * @brief Fixed size buffer queue API.
 **/
#pragma once
#include <Common.h>
#include "Array.h"

/**
 * @struct Queue
 * @brief  Queue with fixed size buffer.
 * @note Can be casted safely to an Array.
 * @field array:
 *	@type Array
 *	@brief Inherits array properties.
 * @field last_element_index:
 *	@type uint64
 *	@brief index of the last element.
 **/
typedef struct _Queue {
    Array array;
    uint64 last_element_index;
} Queue;


/**
 * @func LT_QueueCreate
 * @brief create a queue with the data buffer on the heap.
 * @param size:
 *	@type uint64
 *	@brief size in bytes of the data buffer.
 * @param typeSize:
 *	@type uint64
 *	@brief size in bytes of a data element.
 **/
Queue LT_QueueCreate(uint64 size, uint64 typeSize);

void* LT_QueuePop(Queue* queue);

void LT_QueuePush(Queue* queue, void* element);
