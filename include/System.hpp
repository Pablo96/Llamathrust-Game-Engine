/**
 * @file System.h
 * @author Pablo Narvaja
 * @brief In here is all code related to system(OS) access.
 **/
#pragma once
#include <Common.hpp>
typedef void *SharedLib;

/**
 * @func LT_LoadSharedLibrary
 * @brief Loads a system or user shared library
 * @param in_name
 *   @type const char pointer
 *   @brief name of the shared library without extension
 **/
SharedLib LT_LoadSharedLibrary(const char *in_name);

/**
 * @func LT_GetProcAddrFromSharedLib
 * @brief Gets the address of an asset in the specified shared library
 * @param in_lib
 *   @type const void pointer
 *   @brief shared library
 * @param in_name
 *   @type const char* pointer
 *   @brief name of the asset in the shared library
 **/
void *LT_GetProcAddrFromSharedLib(const SharedLib in_lib, const char *in_name);
