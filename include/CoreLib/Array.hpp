/**
 * @file Array.h
 * @author Pablo Narvaja
 * @brief Fixed Array API.
 **/
#pragma once
#include <Common.hpp>

namespace LT {
	/**
	 * @struct Array
	 * @brief Fixed size array.
	 * @field data:
	 *	@type void pointer
	 *	@brief data buffer.
	 * @field size:
	 *	@type const uint64
	 *	@brief data buffer size in bytes.
	 * @field typeSize:
	 *	@type const uint32
	 *	@brief data element size in bytes.
	 **/
	class Array {
		void* data;
		const uint64 size;
		const uint64 typeSize;
	
	protected:
		uint64 elements_count;

	public:

		/**
		 * @func LT_ArrayCreate
		 * @brief create an array with the data buffer on the heap.
		 * @param count:
		 *	@type uint64
		 *	@brief max element count of the data buffer.
		 * @param typeSize:
		 *	@type uint64
		 *	@brief size in bytes of a data element.
		 **/
		Array(const uint64 count, const uint64 typeSize);

		/**
		 * @func Constructor on stack
		 * @brief create an array with the dataBuffer (created on stack) as data buffer.
		 * @param dataBufer:
		 *	@type void pointer
		 *	@brief data buffer.
		 * @param count:
		 *	@type uint64
		 *	@brief max element count of the data buffer.
		 * @param typeSize:
		 *	@type uint64
		 *	@brief size in bytes of a data element.
		 * @return Array
		 *  @brief array object
		 **/
		Array(void* dataBuffer, const uint64 count, const uint64 typeSize);


		/**
		* Destructor
		*/
		~Array();

		/**
		 * @func Count
		 * @brief get the element count of the array.
		 * @return uint64
		 **/
		uint64 Count();


		/**
		 * @func GetElement
		 * @brief get the element at the specified index.
		 * @param array:
		 *	@type const Array pointer
		 *	@brief the array to operate on.
		 * @param index:
		 *	@type const uint64
		 *	@brief index from which to retrieve the element.
		 * @return void pointer
		 *  @brief pointer to the element at that index.
		 **/
		void* GetElement(const uint64 index);

		/**
		 * @func SetElement
		 * @brief copy the data pointed by the element param
		 *          to the specified index in the array.
		 * @param array:
		 *	@type const Array pointer
		 *	@brief the array to operate on.
		 * @param index:
		 *	@type const uint64
		 *	@brief index from which to retrieve the element.
		 * @param element:
		 *	@type const void pointer
		 *	@brief the element to set at the index position.
		 * @return void
		 **/
		void SetElement(const uint64 index, void* element);


		Array operator=(const Array& array);
	};
}
