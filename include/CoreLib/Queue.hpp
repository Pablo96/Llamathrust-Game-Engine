#pragma once
#include "Array.hpp"
#include <Common.hpp>

namespace LT {
	/**
	 * @struct Queue
	 * @brief  Queue (FiFo) with fixed size buffer.
	 * @note Can be casted safely to an Array.
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
	class Queue : public Array {
		uint64 last_element_index;
		uint64 first_element_index;
		bool isEmpty;

	public:
		Queue(uint64 count, uint64 type_size);

		/**
		 * @func Pop
		 * @brief Gets and delete the first element of the queue.
		 * @param queue:
		 *	@type Queue pointer
		 *	@brief queue to operate on.
		 * @return void pointer
		 *  @brief The first element.
		 **/
		void* Pop();

		void* GetFirst();

		void Push(void* element);

		bool IsEmpty();
	};

}
