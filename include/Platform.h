#pragma once
#include "Common.h"

typedef void* (*LoadProc)(const char* name);
typedef void (*SwapBuffersFunc)(void);

// Forward declaration
typedef struct _Thread Thread;
typedef uint64 (*ThreadFuncWrapper)(void* name);


// WINDOWS
#ifdef LT_WINDOWS
#define LT_EXPORT __declspec(dllexport)
typedef struct HWND__ *HWND;
typedef struct HDC__ *HDC;
typedef void* HANDLE;

typedef struct {
  HANDLE  handle;
  const unsigned long id;
} ThreadWin;

typedef struct {
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
#endif // End Windows

#define LT_Init(GameConstructor) LT_EXPORT void* GetInitGame() {\
Game* game = (Game*) malloc(sizeof(Game)); return GameConstructor(game); }

extern Window window;
extern LoadProc InitOpenGL(void);
extern SwapBuffersFunc GetPlatformSwapBuffer(void);

extern void PlatformInitInput(int32* in_keyStates);
extern uint8 PlatformGetKeyState(int32 keyState);

extern void* PlatformLoadSharedLib(const char* name);
extern void* PlatformGetProc(const void* in_lib, const char* in_name);

extern Thread* PlatformThreadCreate(ThreadFuncWrapper funcWrapper, void* paramter, const char* name);
extern void PlatformThreadJoin(const Thread* thread);
extern void PlatformThreadSleep(const Thread* thread, const uint64 miliseconds);
extern Thread* PlatformThreadGetCurrent(void);
extern void PlatformThreadExit(const int16 exit_code);
extern void PlatformThreadGetExitCode(Thread* thread);
extern void PlatformThreadDestroy(Thread* thread);
