/**
 * @file Stack.h
 * @author Pablo Narvaja
 * @brief Fixed size buffer Stack API.
 **/
#pragma once
#include "Array.h"
#include <Common.h>

/**
 * @struct Stack
 * @brief  Stack (FiLo) with fixed size buffer.
 * @note Can be casted safely to an Array.
 * @field array:
 *	@type Array
 *	@brief Inherits array properties.
 * @field last_element_index:
 *	@type uint64
 *	@brief current count of elements.
 * @field isEmpty:
 *	@type bool
 *	@brief if empty is set to LT_TRUE.
 **/
typedef struct _Stack {
  Array array;
  uint64 elements_count;
  bool isEmpty;
} Stack;

/**
 * @func LT_StackCreate
 * @brief create a Stack with the data buffer on the heap.
 * @param size:
 *	@type uint64
 *	@brief size in bytes of the data buffer.
 * @param typeSize:
 *	@type uint64
 *	@brief size in bytes of a data element.
 **/
Stack LT_StackCreate(uint64 size, uint64 typeSize);

/**
 * @func LT_StackDestroy
 * @brief free the data buffer memory.
 * @note DO NOT use with an stack created on stack.
 * @param stack:
 *	@type const Array pointer
 *	@brief the array to operate on.
 **/
void LT_StackDestroy(Stack *stack);

/**
 * @func LT_StackPop
 * @brief Get and delete the top element from the Stack.
 * @note Should check if is not empty,
 *       if stack is empty return of this function is not defined.
 * @param stack:
 *	@type Stack pointer
 *	@brief the stack to operate on
 * @return void pointer
 *  @brief The last element.
 **/
void *LT_StackPop(Stack *stack);

/**
 * @func LT_StackTop
 * @brief Get the top element from the Stack.
 * @note Should check if is not empty,
 *       if stack is empty return of this function is not defined.
 * @param stack:
 *	@type Stack pointer
 *	@brief the stack to operate on
 * @return void pointer
 *  @brief The last element.
 **/
void *LT_StackTop(Stack *stack);

/**
 * @func LT_StackPush
 * @brief Push an element to the stack.
 * @param stack:
 *	@type Stack pointer
 *	@brief the stack to operate on
 * @param element:
 *	@type void pointer
 *	@brief element to put at top of the stack
 **/
void LT_StackPush(Stack *stack, void *element);
