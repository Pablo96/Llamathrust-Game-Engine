#pragma once
#include "Array.hpp"
#include <Common.hpp>

/**
 * @struct Queue
 * @brief  Queue (FiFo) with fixed size buffer.
 * @note Can be casted safely to an Array.
 * @field array:
 *	@type Array
 *	@brief Inherits array properties.
 * @field last_element_index:
 *	@type uint64
 *	@brief index of the last element.
 * @field firs_element_index:
 *	@type uint64
 *	@brief index of the first element.
 * @field count:
 *	@type uint64
 *	@brief cashed reserved elements count.
 * @field isEmpty:
 *	@type bool
 *	@brief if empty is set to LT_TRUE.
 **/
typedef struct _Queue {
  Array array;
  uint64 last_element_index;
  uint64 first_element_index;
  uint64 count;
  bool isEmpty;
} Queue;

Queue LT_QueueCreate(const uint64 size, const uint64 elementSize);

void LT_QueueDestroy(Queue *queue);

/**
 * @func LT_QueuePop
 * @brief Gets and delete the first element of the queue.
 * @param queue:
 *	@type Queue pointer
 *	@brief queue to operate on.
 * @return void pointer
 *  @brief The first element.
 **/
void *LT_QueuePop(Queue *queue);

void *LT_QueueFirst(Queue *queue);

void LT_QueuePush(Queue *queue, void *element);
