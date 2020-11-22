/**
 * @file String.h
 * @author Pablo Narvaja
 * @brief Strings API.
 **/
#pragma once
#include <Common.hpp>

#define LT_STACK_STRING(name, max_length) char name[max_length]

/**
 * @struct String
 * @brief Dynamic string.
 * @field data:
 *	@type char pointer
 *	@brief buffer where string is stored.
 * @field size:
 *	@type uint64 size
 *	@brief current length of the string.
 **/
typedef struct _string {
  char *data;
  uint64 size;
} String;

extern void LT_StringCreate(const char *value);
extern void LT_StringDelete(String *string);
extern void LT_StringMove(String *string, String *other);
