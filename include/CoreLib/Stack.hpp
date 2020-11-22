/**
 * @file Stack.h
 * @author Pablo Narvaja
 * @brief Fixed size buffer Stack API.
 **/
#pragma once
#include "Array.hpp"
#include <Common.hpp>

namespace LT {
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
	class Stack : public Array {
		bool isEmpty;

	public:
		/**
		 * @func Constructor
		 * @brief create a Stack with the data buffer on the heap.
		 * @param size:
		 *	@type uint64
		 *	@brief size in bytes of the data buffer.
		 * @param typeSize:
		 *	@type uint64
		 *	@brief size in bytes of a data element.
		 **/
		Stack(const uint64 size, const uint64 typeSize);
		
		/**
		 * @func LT_StackPop
		 * @brief Get and delete the top element from the Stack.
		 * @note Should check if is not empty,
		 *       if stack is empty return of this function is not defined.
		 * @return void pointer
		 *  @brief The last element.
		 **/
		void* Pop();

		/**
		 * @func Top
		 * @brief Get the top element from the Stack.
		 * @note Should check if is not empty,
		 *       if stack is empty return of this function is not defined.
		 * @return void pointer
		 *  @brief The last element.
		 **/
		void* Top();

		/**
		 * @func Push
		 * @brief Push an element to the stack.
		 * @param element:
		 *	@type void pointer
		 *	@brief element to put at top of the stack
		 **/
		void Push(void* element);

		bool IsEmpty();
	};
}
