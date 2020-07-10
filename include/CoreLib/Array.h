/**
 * @file Array.h
 * @author Pablo Narvaja
 * @brief Fixed Array API.
 **/
#pragma once
#include <Common.h>

/**
 * @struct Array
 * @brief Fixed size array.
 * @field data:
 *	@type void const pointer
 *	@brief data buffer.
 * @field size:
 *	@type const uint64
 *	@brief data buffer size in bytes.
 * @field typeSize:
 *	@type const uint32
 *	@brief data element size in bytes.
 **/
typedef struct _Array {
    void const* data;
    const uint64 size;
    const uint64 typeSize;
} Array;


/**
 * @func LT_ArrayCreate
 * @brief create an array with the data buffer on the heap.
 * @param size:
 *	@type uint64
 *	@brief size in bytes of the data buffer.
 * @param typeSize:
 *	@type uint64
 *	@brief size in bytes of a data element.
 **/
Array LT_ArrayCreate(uint64 size, uint64 typeSize);

/**
 * @func LT_ArrayStackCreate
 * @brief create an array with the dataBuffer (created on stack) as data buffer.
 * @param dataBufer:
 *	@type void pointer
 *	@brief data buffer.
 * @param size:
 *	@type uint64
 *	@brief size in bytes of the data buffer.
 * @param typeSize:
 *	@type uint64
 *	@brief size in bytes of a data element.
 * @return Array
 *  @brief array object
 **/
Array LT_ArrayStackCreate(void* dataBuffer, uint64 size, uint64 typeSize);

/**
 * @func LT_ArrayCount
 * @brief get the element count of the array.
 * @param array:
 *	@type const Array pointer
 *	@brief the array to operate on.
 * @return uint64
 **/
uint64 LT_ArrayCount(const Array* array);

/**
 * @func LT_ArrayDestroy
 * @brief free the data buffer memory.
 * @note DO NOT use with an array stack created.
 * @param array:
 *	@type const Array pointer
 *	@brief the array to operate on.
 **/
void LT_ArrayDestroy(const Array* array);

/**
 * @func LT_ArrayGetElement
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
void* LT_ArrayGetElement(const Array* array, const uint64 index);

/**
 * @func LT_ArrayGetElement
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
void LT_ArraySetElement(const Array* array, const uint64 index, void* element);
