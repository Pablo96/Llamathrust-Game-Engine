#include "Platform.hpp"
#include <System.hpp>

namespace LT {
	SharedLib LoadSharedLibrary(const char* in_name) {
		return PlatformLoadSharedLib(in_name);
	}

	void* GetProcAddrFromSharedLib(const SharedLib in_lib, const char* in_name) {
		return PlatformGetProc(in_lib, in_name);
	}
}
