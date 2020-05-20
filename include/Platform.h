#pragma once
#include "Common.h"

#ifdef _WIN64
    #define LT_WINDOWS
#elif defined(_WIN32)
    #error "Windows 32bit is not supported!"
#elif defined(__ANDROID__)
    #define LT_ANDROID
#elif defined(__linux__)
    #define LT_LINUX
#elif defined(__APPLE__)
     #error "APPLE is not supported!"
#endif


// Window handle
#ifdef LT_WINDOWS
typedef struct HWND__ *HWND;
typedef struct HDC__ *HDC;

typedef struct _Window {
    HWND handle;
    HDC device;
} Window;
#endif


extern uint32 windowsCount;
extern Window* windowsVec;

/* Creates a window (width, height, title)
 * NOTE: Remember to define this variable in the platform implementation
 * */
extern void (*LT_CreateWindow)(int32, int32, const char*);
