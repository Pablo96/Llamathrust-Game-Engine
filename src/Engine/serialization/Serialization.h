/**
 * @file Serialization.h
 * @author Pablo Narvaja
 * @brief TODO.
 **/
#pragma once
#include <Common.h>

typedef void (*SerializableWriteFunc)(const byte* in_stream);
typedef void (*SerializableReadFunc)(byte* in_stream);

/**
 * @struct Serializable
 * @brief Serializable object
 * @field _data:
 *	@type void pointer
 *	@brief object to serialize
 * @field write:
 *	@type void (*SerializableWriteFunc)(const byte* in_stream)
 *	@brief Function to serialize the _data
 * @field read:
 *	@type void (*SerializableReadFunc)(byte* in_stream);
 *	@brief Function to deserialize the _data
 **/
typedef struct _Serializable {
    void* _data;
    SerializableWriteFunc write;
    SerializableReadFunc read;
} Serializable;

/**
 * @def LT_SERIALIZATION_WRITE_FUNCTION(type)
 * @brief Function header for a write serialization function.
 * @param type
 *      @brief Object type name
 * @note Access this function as LT_WriteTypeName where TypeName is the type param passed.
 * @example LT_REGISTER_WRITE_FUNCTION(int32) is accessed as void LT_Write(byte* in_stream, int* in_object)
 **/
#define LT_SERIALIZATION_WRITE_FUNCTION(type) void LT_Write(byte* in_stream, type* in_object)

/**
 * @def LT_SERIALIZATION_PRIMITIVE_WRITE_FUNCTION(type)
 * @brief Function header for a write serialization function.
 * @param type
 *      @brief Object type name
 * @param typeFuncName
 *      @brief Object type name
 * @note Access this function as LT_WriteTypeFuncName where TypeName is the typeFuncName param passed.
 * @example LT_REGISTER_WRITE_FUNCTION(Int32, int32) is accessed as void LT_WriteInt32(byte* in_stream, type* in_object)
 **/
#define LT_SERIALIZATION_PRIMITIVE_WRITE_FUNCTION(typeFuncName, type) void MAKE_FN_NAME(LT_Write, typeFuncName)(byte* in_stream, type* in_object)

/**
 * @func GetSerializableOf
 * @brief Creates a serializable object from data object with the corresponding funcitons.
 * @param data:
 *	@type void pointer
 *	@brief Object to be serialized
 * @param writeFunc:
 *	@type SerializableWriteFunc
 *	@brief Function that serialize the object
 * @param data:
 *	@type SerializableReadFunc
 *	@brief Function that deserialize the object
 **/
extern Serializable* GetSerializableOf(void* data, SerializableWriteFunc writeFunc, SerializableReadFunc readFunc);

/**
 * Register primitive types
 **/

extern LT_SERIALIZATION_PRIMITIVE_WRITE_FUNCTION(Int8, int8);
extern LT_SERIALIZATION_PRIMITIVE_WRITE_FUNCTION(Int16, int16);
extern LT_SERIALIZATION_PRIMITIVE_WRITE_FUNCTION(Int32, int32);
extern LT_SERIALIZATION_PRIMITIVE_WRITE_FUNCTION(Int64, int64);
extern LT_SERIALIZATION_PRIMITIVE_WRITE_FUNCTION(Uint8, uint8);
extern LT_SERIALIZATION_PRIMITIVE_WRITE_FUNCTION(Uint16, uint16);
extern LT_SERIALIZATION_PRIMITIVE_WRITE_FUNCTION(Uint32, uint32);
extern LT_SERIALIZATION_PRIMITIVE_WRITE_FUNCTION(Uint32, uint32);
