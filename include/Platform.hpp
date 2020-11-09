#pragma once
#include "Common.hpp"

namespace LT {
    typedef void *(*LoadProc)(const char *name);
    typedef void (*SwapBuffersFunc)(void);

    // Forward declaration
    typedef struct Thread;
    typedef struct ThreadLock;
    typedef struct NetSocket;
    typedef struct NetAddress;
}

// WINDOWS
#ifdef LT_WINDOWS
#define LT_EXPORT __declspec(dllexport)
typedef struct HWND__ *HWND;
typedef struct HDC__ *HDC;
typedef void *HANDLE;

namespace LT {
    typedef uint64(*ThreadFuncWrapper)(void* name);

    struct ThreadWin {
        HANDLE handle;
        const unsigned long id;

        ThreadWin(uint32 id) : id(id), handle(nullptr) {}
    };

    struct Window {
        HWND handle;
        HDC device;

        Window() : handle(nullptr), device(nullptr) {}
    };
}
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
typedef void* (*LT::ThreadFuncWrapper)(void *name);
namespace LT {
    typedef struct {
        union {
            uint32* id;
            uint32* handle;
        }
    } ThreadLinux;

    typedef struct {
    } Window;
}
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
extern LT::Window window;
extern LT::LoadProc InitOpenGL(void);
extern LT::SwapBuffersFunc GetPlatformSwapBuffer(void);

// INPUT
extern void PlatformInitInput(int32 *in_keyStates);
extern uint8 PlatformGetKeyState(int32 keyState);

// DYNAMIC LIB
extern void *PlatformLoadSharedLib(const char *name);
extern void *PlatformGetProc(const void *in_lib, const char *in_name);

// NETWORKING
extern void PlatformNetAddress(LT::NetAddress *address);
extern void PlatformNetAddressDestroy(LT::NetAddress *address);
extern void PlatformSocketCreate(LT::NetSocket *socket);
extern bool PlatformSocketBind(const LT::NetSocket *socket);
extern bool PlatformSocketListen(const LT::NetSocket *socket);
extern LT::NetSocket *PlatformSocketAccept(const LT::NetSocket *in_socket);
extern void PlatformSocketClose(LT::NetSocket *socket);
extern bool PlatformSocketConnect(LT::NetSocket *socket, const LT::NetAddress *address);
extern bool PlatformSocketConnClose(const LT::NetSocket *socket);
extern bool PlatformSocketSend(const LT::NetSocket *socket, const char *msg,
                               const uint32 msg_len);
extern bool PlatformSocketRecieve(const LT::NetSocket *socket, char *msg,
                                  uint32 *msg_len);

// THREADING
namespace LT {
    struct Platform {
        static LT::Thread* PlatformThreadCreate(LT::Thread* thread, LT::ThreadFuncWrapper funcWrapper);
        static void PlatformThreadStart(const LT::Thread* thread);
        static void PlatformGetCurrent(const LT::Thread* thread);
        static void PlatformThreadJoin(const LT::Thread* thread);
        static void PlatformThreadSleep(const LT::Thread* thread, const uint64 miliseconds);
        static void PlatformThreadExit(const int16 exit_code);
        static void PlatformThreadGetExitCode(LT::Thread* thread);
        static void PlatformThreadDestroy(LT::Thread* thread);

        static LT::ThreadLock* PlatformThreadLockCreate(void);
        static void PlatformThreadLockLock(LT::ThreadLock* lock);
        static void PlatformThreadLockUnock(LT::ThreadLock* lock);
        static void PlatformThreadLockDestroy(LT::ThreadLock* lock);
    };
}