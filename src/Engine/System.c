#include <System.h>
#include "Platform.h"

SharedLib LT_LoadSharedLibrary(const char* in_name) {
    return PlatformLoadSharedLib(in_name);
}

void* LT_GetProcAddrFromSharedLib(const SharedLib in_lib, const char* in_name) {
    return PlatformGetProc(in_lib, in_name);
}