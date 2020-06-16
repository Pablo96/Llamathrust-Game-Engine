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

typedef void* (*LoadProc)(const char* name);
typedef void (*SwapBuffersFunc)(void);

// WINDOWS
#ifdef LT_WINDOWS
typedef struct HWND__ *HWND;
typedef struct HDC__ *HDC;

typedef struct _Window {
  HWND handle;
  HDC device;
} Window;

/**
 * @def LT_CREATEGAME
 * @brief Generates the entrypoint for the game library
 * @note Only needed in one .c file per shared library of a game
 * @note A Game is soposed to be in one library but it could be in multiple ones if that lib loads another
 **/
#define LT_CREATEGAME()\
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {\
switch( fdwReason ) {\
  case DLL_PROCESS_ATTACH: break;\
  case DLL_THREAD_ATTACH: break;\
  case DLL_THREAD_DETACH: break;\
  case DLL_PROCESS_DETACH: break;\
} return TRUE;}
#endif

extern Window window;
extern void LT_CloseWindow(void);
extern LoadProc InitOpenGL(void);
extern SwapBuffersFunc GetPlatformSwapBuffer(void);
extern void InitPlatformInput(int32* key_states);
extern uint8 GetPlatformKeyState(int32 key_state);
