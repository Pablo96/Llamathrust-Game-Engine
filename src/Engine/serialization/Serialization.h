/**
 * @file Serialization.h
 * @author Pablo Narvaja
 * @brief TODO.
 **/
#include <Common.h>

/**
 * @def LT_REGISTER_WRITE_FUNCTION(type)
 * @brief Function header for a write serialization function.
 * @param type
 *      @brief Object type name
 * @note Access this function as LT_WriteTypeName where TypeName is the type param passed.
 * @example LT_REGISTER_WRITE_FUNCTION(int32) is accessed as void LT_Writeint32(byte* in_stream, type* in_object)
 **/
#define LT_SERIALIZATION_WRITE_FUNCTION(type) void MAKE_FN_NAME(LT_Write, type)(byte* in_stream, type* in_object)

/**
 * @def LT_REGISTER_WRITE_FUNCTION(type)
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
