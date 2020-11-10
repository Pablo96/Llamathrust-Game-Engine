#pragma once
#include "Common.hpp"

namespace LT {
    typedef void *(*LoadProc)(const char *name);
    typedef void (*SwapBuffersFunc)(void);

    // Forward declaration
    class Thread;
    class ThreadLock;
    class NetSocket;
    class NetAddress;
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

namespace LT {
    struct Platform {
        // INPUT
        static void InitInput(int32 *in_keyStates);
        static uint8 GetKeyState(int32 keyState);

        // DYNAMIC LIB
        static void *LoadSharedLib(const char *name);
        static void *GetProc(const void *in_lib, const char *in_name);

        // NETWORKING
        static void NetAddressCreate(LT::NetAddress *address);
        static void NetAddressDestroy(LT::NetAddress *address);
        static void SocketCreate(LT::NetSocket *socket);
        static bool SocketBind(const LT::NetSocket *socket);
        static bool SocketListen(const LT::NetSocket *socket);
        static LT::NetSocket *SocketAccept(const LT::NetSocket *in_socket);
        static void SocketClose(LT::NetSocket *socket);
        static bool SocketConnect(LT::NetSocket *socket, const LT::NetAddress *address);
        static bool SocketConnClose(const LT::NetSocket *socket);
        static bool SocketSend(const LT::NetSocket *socket, const char *msg,
                                const uint32 msg_len);
        static bool SocketRecieve(const LT::NetSocket *socket, char *msg,
                                  uint32 *msg_len);

        // THREADING
        static LT::Thread* ThreadCreate(LT::Thread* thread, LT::ThreadFuncWrapper funcWrapper);
        static void ThreadStart(const LT::Thread* thread);
        static void GetCurrent(const LT::Thread* thread);
        static void ThreadJoin(const LT::Thread* thread);
        static void ThreadSleep(const LT::Thread* thread, const uint64 miliseconds);
        static void ThreadExit(const int16 exit_code);
        static void ThreadGetExitCode(LT::Thread* thread);
        static void ThreadDestroy(LT::Thread* thread);

        static LT::ThreadLock* ThreadLockCreate(void);
        static void ThreadLockLock(LT::ThreadLock* lock);
        static void ThreadLockUnock(LT::ThreadLock* lock);
        static void ThreadLockDestroy(LT::ThreadLock* lock);
    };
}