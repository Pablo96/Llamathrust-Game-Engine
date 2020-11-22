#include "Platform.hpp"
#include <System.hpp>

namespace LT {
	SharedLib LoadSharedLibrary(const char* in_name) {
		return Platform::LoadSharedLib(in_name);
	}

	void* GetProcAddrFromSharedLib(const SharedLib in_lib, const char* in_name) {
		return Platform::GetProc(in_lib, in_name);
	}
}
