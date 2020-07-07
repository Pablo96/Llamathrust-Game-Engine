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
#define LT_EXPORT __declspec(dllexport)
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

#define LT_Init(GameConstructor) LT_EXPORT void* GetInitGame() {\
Game* game = (Game*) malloc(sizeof(Game)); return GameConstructor(game); }

typedef struct _Socket Socket;

// GRAPHICS

extern Window window;
extern LoadProc InitOpenGL(void);
extern SwapBuffersFunc GetPlatformSwapBuffer(void);

// INPUT

extern void PlatformInitInput(int32* in_keyStates);
extern uint8 PlatformGetKeyState(int32 keyState);

// DYNAMIC LIB

extern void* PlatformLoadSharedLib(const char* name);
extern void* PlatformGetProc(const void* in_lib, const char* in_name);

// NETWORKING
extern void PlatformSocketCreate(const Socket* socket, bool is_server = LT_FALSE, bool is_IPv6 = LT_FALSE);
extern bool PlatformSocketBind(const Socket* socket);
extern Socket* PlatformSocketListenAndAccept(const Socket* socket);
extern void PlatformSocketClose(const Socket* socket);
extern bool PlatformSocketConnClose(const Socket* socket);
extern bool PlatformSocketSend(const Socket* socket, const char* msg, const uint32 msg_len);
extern bool PlatformSocketRecieve(const Socket* socket, char* msg, uint32* msg_len);
