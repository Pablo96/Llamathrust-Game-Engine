/**
 * @file System.h
 * @author Pablo Narvaja
 * @brief In here is all code related to system(OS) access.
 **/
#pragma once
#include <Common.hpp>

namespace LT {
	typedef void* SharedLib;

	/**
	 * @func LoadSharedLibrary
	 * @brief Loads a system or user shared library
	 * @param in_name
	 *   @type const char pointer
	 *   @brief name of the shared library without extension
	 **/
	SharedLib LoadSharedLibrary(const char* in_name);

	/**
	 * @func GetProcAddrFromSharedLib
	 * @brief Gets the address of an asset in the specified shared library
	 * @param in_lib
	 *   @type const void pointer
	 *   @brief shared library
	 * @param in_name
	 *   @type const char* pointer
	 *   @brief name of the asset in the shared library
	 **/
	void* GetProcAddrFromSharedLib(const SharedLib in_lib, const char* in_name);
}
