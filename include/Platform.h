#pragma once
#include "Common.h"

typedef void *(*LoadProc)(const char *name);
typedef void (*SwapBuffersFunc)(void);

// Forward declaration
typedef struct _Thread Thread;
typedef struct _ThreadLock ThreadLock;
typedef struct _NetSocket NetSocket;
typedef struct _NetAddress NetAddress;

// WINDOWS
#ifdef LT_WINDOWS
#define LT_EXPORT __declspec(dllexport)
typedef uint64 (*ThreadFuncWrapper)(void *name);
typedef struct HWND__ *HWND;
typedef struct HDC__ *HDC;
typedef void *HANDLE;

typedef struct {
  HANDLE handle;
  const unsigned long id;
} ThreadLinuxThreadWin;

typedef struct {
  HWND handle;
  HDC device;
} Window;

/**
 * @def LT_CREATEGAME
 * @brief Generates the entrypoint for the game library
 * @note Only needed in one .c file per shared library of a game
 * @note A Game is soposed to be in one library but it could be in multiple ones
 *if that lib loads another
 **/
#define LT_CREATEGAME()\
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {\
switch( fdwReason ) {\
  case DLL_PROCESS_ATTACH: break;\
  case DLL_THREAD_ATTACH: break;\
  case DLL_THREAD_DETACH: break;\
  case DLL_PROCESS_DETACH: break;\
} return TRUE;}
#elif defined(LT_LINUX)
#define LT_EXPORT __attribute__((visibility("default")))
typedef void* (*ThreadFuncWrapper)(void *name);
typedef struct {
  union {
    uint32 *id;
    uint32 *handle;
  }
} ThreadLinux;

typedef struct {
} Window;

/**
 * @def LT_CREATEGAME
 * @brief Generates the entrypoint for the game library
 * @note Only needed in one .c file per shared library of a game
 * @note A Game is soposed to be in one library but it could be in multiple ones if that lib loads another
 **/
#define LT_CREATEGAME()
#endif

#define LT_Init(GameConstructor)                                               \
  LT_EXPORT void *GetInitGame() {                                              \
    Game *game = (Game *)malloc(sizeof(Game));                                 \
    return GameConstructor(game);                                              \
  }

// GRAPHICS
extern Window window;
extern LoadProc InitOpenGL(void);
extern SwapBuffersFunc GetPlatformSwapBuffer(void);

// INPUT
extern void PlatformInitInput(int32 *in_keyStates);
extern uint8 PlatformGetKeyState(int32 keyState);

// DYNAMIC LIB
extern void *PlatformLoadSharedLib(const char *name);
extern void *PlatformGetProc(const void *in_lib, const char *in_name);

// NETWORKING
extern void PlatformNetAddress(NetAddress *address);
extern void PlatformNetAddressDestroy(NetAddress *address);
extern void PlatformSocketCreate(NetSocket *socket);
extern bool PlatformSocketBind(const NetSocket *socket);
extern bool PlatformSocketListen(const NetSocket *socket);
extern NetSocket *PlatformSocketAccept(const NetSocket *in_socket);
extern void PlatformSocketClose(NetSocket *socket);
extern bool PlatformSocketConnect(NetSocket *socket, const NetAddress *address);
extern bool PlatformSocketConnClose(const NetSocket *socket);
extern bool PlatformSocketSend(const NetSocket *socket, const char *msg,
                               const uint32 msg_len);
extern bool PlatformSocketRecieve(const NetSocket *socket, char *msg,
                                  uint32 *msg_len);

// THREADING
extern Thread *PlatformThreadCreate(const Thread *thread, ThreadFuncWrapper funcWrapper);
extern void PlatformThreadStart(const Thread *thread);
extern void PlatformGetCurrent(const Thread *thread);
extern void PlatformThreadJoin(const Thread *thread);
extern void PlatformThreadSleep(const Thread *thread, const uint64 miliseconds);
extern void PlatformThreadExit(const int16 exit_code);
extern void PlatformThreadGetExitCode(Thread *thread);
extern void PlatformThreadDestroy(Thread *thread);

extern ThreadLock *PlatformThreadLockCreate(void);
extern void PlatformThreadLockLock(ThreadLock *lock);
extern void PlatformThreadLockUnock(ThreadLock *lock);
extern void PlatformThreadLockDestroy(ThreadLock *lock);
