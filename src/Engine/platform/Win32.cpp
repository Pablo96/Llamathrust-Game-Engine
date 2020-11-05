#include "Win32.hpp"
#ifdef LT_WINDOWS
#include "../Engine.hpp"
#include "../Input.hpp"
#include "../Performance.hpp"
#include "../networking/Socket.hpp"
#include "../ArgsParsing.hpp"
#include <ErrorCodes.hpp>
#include <Networking.hpp>
#include <Thread.hpp>

#include <log.hpppp>

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <wingdi.h>

#include <gl/GL.h>
#include <gl/wglext.hpp>

#include <stdlib.h>
#include <string.h>

#ifdef LT_CLANG
#include <stdnoreturn.h>
#elif defined(LT_VS)
#define noreturn
#endif

// Input
static const uint64 win32KeyStatesSize = VK_RMENU + 1;
static uint8 *win32KeyStates;

// Windows
LT_Window_t window;
static BOOL shouldClose = FALSE;

// Networking
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

// Win32
static HINSTANCE hInstance;
static HINSTANCE glInstance;
static HGLRC modernGLcontext;
#ifdef LT_EDITOR
static const LPTSTR CLASS_NAME = "EditorWindow";
#define WINDOW_TITLE "Editor x64"
#define WINDOW_STYLE WS_POPUP
#define WINDOW_STYLE_EX WS_EX_ACCEPTFILES
#else
static const LPTSTR CLASS_NAME = "GameWindow";
#define WINDOW_TITLE "Game x64"
#define WINDOW_STYLE WS_OVERLAPPED
#define WINDOW_STYLE_EX 0
#endif
static const LPTSTR GHOST_CLASS_NAME = "GhostWindow";
noreturn static void Win32HandleError(int32 in_exitCode);

#ifndef LT_NO_MAIN // used for running tests
int main(int32 argc, const char **argv) {
  // Get handle to this executable
  hInstance = GetModuleHandle(NULL);

  //-----------------------------------------------------------------
  // Parse command line arguments
  //-----------------------------------------------------------------
  const ConfigArgs *config = NULL;
  if (argc > 1) {
    config = parseArgs(argv, argc);
    const char log_msg[] = "Command line arguments parsed!.";
    log_info(log_msg);
  }

  //-----------------------------------------------------------------
  // Check if is the only instance running
  //-----------------------------------------------------------------

  // Try to open the mutex.
  HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "LlamathrustMutex");

  // If mutex doesnt exists create it and run the engine
  if (!hMutex)
    hMutex = CreateMutex(NULL, FALSE, "LlamathrustMutex");
  // Else there is an instance of the engine running
  else {
    log_fatal("Instance already running");
    return ERROR_INSTANCE_ALREADY_RUNNING;
  }
  //-----------------------------------------------------------------
  // Start setting app the platform layer
  //-----------------------------------------------------------------
  Win32_Helper_RegisterWindowClasses();

  // NetWorking
  Win32_Helper_InitNetworking();

  //-----------------------------------------------------------------
  // Start the engine
  //-----------------------------------------------------------------
  LT_MEASURE_FUNC(Engine_Start(config));

  //-----------------------------------------------------------------
  // Main engine loop
  //-----------------------------------------------------------------
  MSG msg = {0};
  while (shouldClose == FALSE) {
    LT_START_TIME();

    // Retrieve OS messages
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    // Run engine
    double deltaTime = 1.0 / t;
    Engine_Run(deltaTime);

    // Reset key state cache
    memset(win32KeyStates, LT_KEY_UP, win32KeyStatesSize);

    LT_END_TIME();
  }

  //-----------------------------------------------------------------
  // Cleaning at exit
  //-----------------------------------------------------------------
  // Clean up the engine resources if needed
  Engine_Shutdown();

  // Networking
  WSACleanup();

  return 0;
}
#endif

static LPVOID GetErrorMsg(DWORD errorCode) {
  LPVOID lpMsgBuf;
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPSTR)&lpMsgBuf, 0, NULL);
  return lpMsgBuf;
}

//-----------------------------------------------------------------
// System
//-----------------------------------------------------------------
void *PlatformLoadSharedLib(const char *in_name) {
  const size_t size = strlen(in_name);

  char *name_dll = (char*) malloc(size + 5);
  memcpy(name_dll, in_name, size);
  name_dll[size] = 0;

  strcat(name_dll, ".dll");

  void *lib = LoadLibrary(name_dll);
  free(name_dll);

  return lib;
}

void *PlatformGetProc(const void *in_lib, const char *in_name) {
  return (void *)GetProcAddress((HMODULE)in_lib, in_name);
}

//-----------------------------------------------------------------
// Networking
//-----------------------------------------------------------------

void Win32_Helper_InitNetworking(void) {
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    const char log_msg[] = "WSAStartup failed with error: %d";
    log_error(log_msg, iResult);
    return;
  }
}

void PlatformNetAddress(NetAddress *address) {
  ADDRINFOA hints = {
      .ai_family = address->version == ADDR_IPV4 ? AF_INET : AF_INET6,
      .ai_socktype = address->protocol == PROT_UDP ? SOCK_DGRAM : SOCK_STREAM,
      .ai_protocol = address->protocol == PROT_UDP ? IPPROTO_UDP : IPPROTO_TCP,
      .ai_flags = address->willBind ? AI_PASSIVE : 0,
      .ai_canonname = 0,
      .ai_addrlen = 0,
      .ai_addr = 0,
      .ai_next = 0
  };

  PADDRINFOA result = (PADDRINFOA) malloc(sizeof(ADDRINFOA));
  char buffer[7];
  _itoa(address->port, buffer, 10);

  int iResult = getaddrinfo(address->ip, buffer, &hints, &result);
  if (iResult != 0)
    log_error("Could get address.");

  address->reserved = result;
  address->isValid = iResult == 0;
}

void PlatformNetAddressDestroy(NetAddress *address) {
  PADDRINFOA info = (PADDRINFOA) address->reserved;
  freeaddrinfo(info);
}

void PlatformSocketCreate(NetSocket *in_socket) {
  // convert ip name to binary address
  PADDRINFOA hints = (PADDRINFOA) in_socket->address->reserved;
  hints->ai_flags = AI_PASSIVE;

  // Create a SOCKET for connecting to server
  in_socket->reserved = (void*)
      socket(in_socket->address->type == ADDR_IPV4
             ? AF_INET 
             : AF_INET6,
      // SOCK_DGRAM = UDP SOCK_STREAM = TCP
             in_socket->address->protocol == PROT_UDP
                 ? SOCK_DGRAM
                 : SOCK_STREAM, 
             in_socket->address->protocol == PROT_UDP
                 ? IPPROTO_UDP
                 : IPPROTO_TCP
      );

  // Check if it is a valid socket
  in_socket->isValid = reinterpret_cast<SOCKET>(in_socket->reserved) != INVALID_SOCKET;
  if (!in_socket->isValid) {
    log_error("socket failed with error: %ld\n", WSAGetLastError());
  }
}

bool PlatformSocketBind(const NetSocket *in_socket) {
  PSOCKADDR hints = (PSOCKADDR) in_socket->address->reserved;
  int result = bind((SOCKET)in_socket->reserved, hints, sizeof(SOCKADDR));
  if (result != 0) {
    const char *msgError = GetErrorMsg(WSAGetLastError());
    log_error("Couldnt bind socket! Error: %s", msgError);
    LocalFree(msgError);
    return false;
  }
  return true;
}

bool PlatformSocketListen(const NetSocket *in_socket) {
  SOCKET sock = (SOCKET)in_socket->reserved;
  int result = listen(sock, SOMAXCONN);
  if (result != 0) {
    const char *msgError = GetErrorMsg(WSAGetLastError());
    log_error("Listen failed with error: %s", msgError);
    LocalFree(msgError);
    return false;
  }
  return true;
}

NetSocket *PlatformSocketAccept(const NetSocket *in_socket) {
  SOCKET sock = (SOCKET)in_socket->reserved;
  struct sockaddr_in addr = {0};
  SOCKET clientSocket = accept(sock, (struct sockaddr *)&addr, NULL);
  if (clientSocket == INVALID_SOCKET) {
    const char *msgError = GetErrorMsg(WSAGetLastError());
    log_error("Accept failed with error: %s", msgError);
    LocalFree(msgError);
    return NULL;
  }

  NetSocket *client = (NetSocket *)malloc(sizeof(NetSocket));
  NetSocket *address = (NetSocket *)malloc(sizeof(NetAddress));
  LT_NetAddressCreate(address, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port),
                      in_socket->address->version, in_socket->address->type,
                      in_socket->address->protocol, false);
  NetSocket tmp = {
      .address = address,
      .reserved = clientSocket,
  };

  memcpy(client, &tmp, sizeof(NetSocket));
  return client;
}

void PlatformSocketClose(NetSocket *socket) {
  closesocket((SOCKET)socket->reserved);
  socket->reserved = NULL;
}

bool PlatformSocketConnect(NetSocket *in_socket, const NetAddress *address) {
  // Connect to server.
  PADDRINFOA ptr = (PADDRINFOA)address->reserved;
  SOCKET socket = (SOCKET)in_socket->reserved;
  int iResult = connect(socket, ptr->ai_addr, (int)ptr->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
    in_socket->isValid = false;
  }
}

bool PlatformSocketConnClose(const NetSocket *socket) {
  // shutdown the connection since no more data will be sent
  int iResult = shutdown((SOCKET)socket->reserved, SD_SEND);
  if (iResult == SOCKET_ERROR) {
    log_error("Shutdown failed with error: %d", WSAGetLastError());
    return false;
  }
  return true;
}

bool PlatformSocketSend(const NetSocket *socket, const char *msg,
                        const uint32 msg_len) {
  int iResult = send((SOCKET)socket->reserved, msg, (int)msg_len, 0);
  if (iResult == SOCKET_ERROR) {
    log_error("Send failed with error: %d", WSAGetLastError());
    return false;
  }
  return true;
}

bool PlatformSocketRecieve(const NetSocket *socket, char *msg,
                           uint32 *msg_len) {
  int buffer_size =
      *msg_len > MAX_PACKET_SIZE ? (int)MAX_PACKET_SIZE : (int)*msg_len;
  *msg_len = recv((SOCKET)socket->reserved, msg, buffer_size, 0);
  if (*msg_len == 0) {
    if (*msg_len < 0)
      log_error("connection closed or send failed with error: %d",
                WSAGetLastError());
    return false;
  }
  return true;
}

Thread *PlatformThreadCreate(const Thread *thread,
                             ThreadFuncWrapper funcWrapper) {
  DWORD threadID;
  HANDLE threadhandle =
      CreateThread(NULL,             // cant be inherited
                   0,                // Default stack size
                   funcWrapper,      // function that the thread will exec
                   thread,           // parameter to the function
                   CREATE_SUSPENDED, // won't start immediately
                   &threadID);

  if (threadhandle == NULL) {
    log_error("Failed to create thread.");
    Win32HandleError(ERROR_PLATFORM_THREAD_CREATE);
  }

  ThreadWin winThd = {.id = threadID, .handle = threadhandle};

  memcpy(thread->reserved, &winThd, sizeof(ThreadWin));
  return thread;
}

extern void PlatformThreadStart(const Thread *thread) {
  HANDLE handle = ((const ThreadWin *)thread)->handle;
  ResumeThread(handle);
}

void PlatformGetCurrent(const Thread *thread) {
  HANDLE threadhandle = GetCurrentThread();
  DWORD threadID = GetCurrentThreadId();

  ThreadWin winThd = {.id = threadID, .handle = threadhandle};

  memcpy(thread->reserved, &winThd, sizeof(ThreadWin));
}

void PlatformThreadJoin(const Thread *thread) {
  HANDLE handle = ((const ThreadWin *)thread)->handle;
  WaitForSingleObject(handle, INFINITE);
}

void PlatformThreadSleep(const Thread *thread, const uint64 miliseconds) {
  WaitForSingleObject(((const ThreadWin *)thread)->handle, (DWORD)miliseconds);
}

void PlatformThreadExit(const int16 exit_code) { ExitThread(exit_code); }

void PlatformThreadGetExitCode(Thread *thread) {
  ThreadWin *this = (const ThreadWin *)thread;
  DWORD exit_code;
  if (GetExitCodeThread(this->handle, &exit_code) == STILL_ACTIVE) {
    log_error("Thread still active.")
  } else {
    thread->isValid = false;
    thread->exitCode = (int32)exit_code;
  }
}

void PlatformThreadDestroy(Thread *thread) {
  CloseHandle(((const ThreadWin *)thread)->handle);
  thread->isValid = false;
}

ThreadLock *PlatformThreadLockCreate() {
  LPCRITICAL_SECTION lock = malloc(sizeof(CRITICAL_SECTION));
  InitializeCriticalSection(lock);
  return lock;
}

void PlatformThreadLockLock(ThreadLock *lock) { EnterCriticalSection(lock); }

void PlatformThreadLockUnock(ThreadLock *lock) { LeaveCriticalSection(lock); }

void PlatformThreadLockDestroy(ThreadLock *lock) {
  DeleteCriticalSection(lock);
}

//-----------------------------------------------------------------
// Input
//-----------------------------------------------------------------
void PlatformInitInput(int32 *key_states) {
  win32KeyStates = (uint8 *)malloc(win32KeyStatesSize);
  memset(win32KeyStates, LT_KEY_UP, win32KeyStatesSize);

  // Mouse Buttons
  key_states[LT_KEY_MOUSE_BUTTON_L] = VK_LBUTTON;
  key_states[LT_KEY_MOUSE_BUTTON_R] = VK_RBUTTON;
  key_states[LT_KEY_MOUSE_MIDDLE] = VK_MBUTTON;
  key_states[LT_KEY_MOUSE_BUTTON_X1] = VK_XBUTTON1;
  key_states[LT_KEY_MOUSE_BUTTON_X2] = VK_XBUTTON2;

  // Util Keys
  key_states[LT_KEY_BACKSPACE] = VK_BACK;
  key_states[LT_KEY_TAB] = VK_TAB;
  key_states[LT_KEY_DEL] = VK_CLEAR;
  key_states[LT_KEY_ENTER] = VK_RETURN;
  key_states[LT_KEY_SHIFT] = VK_SHIFT;
  key_states[LT_KEY_CONTROL] = VK_CONTROL;
  key_states[LT_KEY_ALT] = VK_MENU;
  key_states[LT_KEY_MENU] = VK_APPS;
  key_states[LT_KEY_PAUSE] = VK_PAUSE;
  key_states[LT_KEY_ESCAPE] = VK_ESCAPE;
  key_states[LT_KEY_SPACEBAR] = VK_SPACE;
  key_states[LT_KEY_PAGE_UP] = VK_PRIOR;
  key_states[LT_KEY_PAGE_DOWN] = VK_NEXT;
  key_states[LT_KEY_END] = VK_END;
  key_states[LT_KEY_HOME] = VK_HOME;
  key_states[LT_KEY_LEFT_ARROW] = VK_LEFT;
  key_states[LT_KEY_UP_ARROW] = VK_RIGHT;
  key_states[LT_KEY_RIGHT_ARROW] = VK_UP;
  key_states[LT_KEY_DOWN_ARROW] = VK_DOWN;
  key_states[LT_KEY_SELECT] = VK_SELECT;
  key_states[LT_KEY_SCREENSHOT] = VK_SNAPSHOT;
  key_states[LT_KEY_EXECUTE] = VK_EXECUTE;
  key_states[LT_KEY_PRINT_SCREEN] = VK_PRINT;
  key_states[LT_KEY_INSERT] = VK_INSERT;
  key_states[LT_KEY_HELP] = VK_HELP;

  // Numers
  key_states[LT_KEY_NUM_0] = 0x30;
  key_states[LT_KEY_NUM_1] = 0x31;
  key_states[LT_KEY_NUM_2] = 0x32;
  key_states[LT_KEY_NUM_3] = 0x33;
  key_states[LT_KEY_NUM_4] = 0x34;
  key_states[LT_KEY_NUM_5] = 0x35;
  key_states[LT_KEY_NUM_6] = 0x36;
  key_states[LT_KEY_NUM_7] = 0x37;
  key_states[LT_KEY_NUM_8] = 0x38;
  key_states[LT_KEY_NUM_9] = 0x39;

  // Letters
  key_states[LT_KEY_A] = 0x41;
  key_states[LT_KEY_B] = 0x42;
  key_states[LT_KEY_C] = 0x43;
  key_states[LT_KEY_D] = 0x44;
  key_states[LT_KEY_E] = 0x45;
  key_states[LT_KEY_F] = 0x46;
  key_states[LT_KEY_G] = 0x47;
  key_states[LT_KEY_H] = 0x48;
  key_states[LT_KEY_I] = 0x49;
  key_states[LT_KEY_J] = 0x50;
  key_states[LT_KEY_K] = 0x51;
  key_states[LT_KEY_L] = 0x52;
  key_states[LT_KEY_M] = 0x53;
  key_states[LT_KEY_N] = 0x54;
  key_states[LT_KEY_O] = 0x55;
  key_states[LT_KEY_P] = 0x56;
  key_states[LT_KEY_Q] = 0x57;
  key_states[LT_KEY_R] = 0x58;
  key_states[LT_KEY_S] = 0x59;
  key_states[LT_KEY_T] = 0x60;
  key_states[LT_KEY_U] = 0x61;
  key_states[LT_KEY_V] = 0x62;
  key_states[LT_KEY_W] = 0x63;
  key_states[LT_KEY_X] = 0x64;
  key_states[LT_KEY_Y] = 0x65;
  key_states[LT_KEY_Z] = 0x66;

  // NUM PAD
  key_states[LT_KEY_NUMPAD0] = VK_NUMPAD0;
  key_states[LT_KEY_NUMPAD1] = VK_NUMPAD1;
  key_states[LT_KEY_NUMPAD2] = VK_NUMPAD2;
  key_states[LT_KEY_NUMPAD3] = VK_NUMPAD3;
  key_states[LT_KEY_NUMPAD4] = VK_NUMPAD4;
  key_states[LT_KEY_NUMPAD5] = VK_NUMPAD5;
  key_states[LT_KEY_NUMPAD6] = VK_NUMPAD6;
  key_states[LT_KEY_NUMPAD7] = VK_NUMPAD7;
  key_states[LT_KEY_NUMPAD8] = VK_NUMPAD8;
  key_states[LT_KEY_NUMPAD9] = VK_NUMPAD9;
  key_states[LT_KEY_MULTIPLY] = VK_MULTIPLY;
  key_states[LT_KEY_PLUS] = VK_ADD;
  key_states[LT_KEY_SEPARATOR] = VK_SEPARATOR;
  key_states[LT_KEY_SUBTRACT] = VK_SUBTRACT;
  key_states[LT_KEY_NUM_DOT] = VK_DECIMAL;
  key_states[LT_KEY_DIVIDE] = VK_DIVIDE;

  // Functions keys
  key_states[LT_KEY_F1] = VK_F1;
  key_states[LT_KEY_F2] = VK_F2;
  key_states[LT_KEY_F3] = VK_F3;
  key_states[LT_KEY_F4] = VK_F4;
  key_states[LT_KEY_F5] = VK_F5;
  key_states[LT_KEY_F6] = VK_F6;
  key_states[LT_KEY_F7] = VK_F7;
  key_states[LT_KEY_F8] = VK_F8;
  key_states[LT_KEY_F9] = VK_F9;
  key_states[LT_KEY_F10] = VK_F10;
  key_states[LT_KEY_F11] = VK_F11;
  key_states[LT_KEY_F12] = VK_F12;
  key_states[LT_KEY_CAPS_LOCK] = VK_CAPITAL;
  key_states[LT_KEY_NUM_LOCK] = VK_NUMLOCK;
  key_states[LT_KEY_SCROLL_LOCK] = VK_SCROLL;

  // Mods
  key_states[LT_KEY_SUPER_L] = VK_LWIN;
  key_states[LT_KEY_SUPER_R] = VK_RWIN;
  key_states[LT_KEY_SHIFT_L] = VK_LSHIFT;
  key_states[LT_KEY_SHIFT_R] = VK_RSHIFT;
  key_states[LT_KEY_ALT_L] = VK_LMENU;
  key_states[LT_KEY_ALT_R] = VK_RMENU;
  key_states[LT_KEY_CONTROL_L] = VK_LCONTROL;
  key_states[LT_KEY_CONTROL_R] = VK_RCONTROL;
}

uint8 PlatformGetKeyState(int32 key_state) { return win32KeyStates[key_state]; }

//-------------------------------------------
// Window
//-------------------------------------------
void LT_CloseWindow(void) { shouldClose = TRUE; }

void Win32SwapBuffer() { SwapBuffers(window.device); }

//-------------------------------------------
// Graphics
//-------------------------------------------


LoadProc InitOpenGL(void) { return Win32InitOpenGL(); }

SwapBuffersFunc GetPlatformSwapBuffer(void) { return Win32SwapBuffer; }

//-----------------------------------------------------------------
// HELPERS
//-----------------------------------------------------------------
void Win32_Helper_CreateWindow(LT_Window_t *wnd, const char *in_wndClassName,
                               int width, int height, const char *title) {
  DWORD styleEx = in_wndClassName == CLASS_NAME ? WINDOW_STYLE_EX : WS_DISABLED;
  DWORD style = in_wndClassName == CLASS_NAME ? WINDOW_STYLE : 0;

  HWND hwnd = CreateWindowEx(styleEx,         // Optional window styles.
                             in_wndClassName, // Window class
                             title,           // Window text
                             style,           // Window style
                             CW_USEDEFAULT, CW_USEDEFAULT, // Window position
                             width, height,                // Window size
                             NULL,                         // Parent window
                             NULL,                         // Menu
                             hInstance,                    // Instance handle
                             NULL // Additional application data
  );

  if (hwnd == NULL) {
    log_fatal("Error creating window of class \"%s\".", in_wndClassName);
    Win32HandleError(ERROR_PLATFORM_WINDOW_CREATION);
  }

  // save the window in the vector
  wnd->handle = hwnd;
  wnd->device = GetDC(hwnd);
}

void Win32_Helper_RegisterWindowClasses() {
  // Register the game window class.
  WNDCLASSEX wcGame = {0};
  wcGame.cbSize = sizeof(wcGame);
  wcGame.hInstance = hInstance;
  wcGame.lpfnWndProc = WindowProc;
  wcGame.lpszClassName = CLASS_NAME;
  wcGame.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

  if (!RegisterClassEx(&wcGame)) {
    log_fatal("Error: Could not register Window Class \"%s\".", CLASS_NAME);
    Win32HandleError(ERROR_PLATFORM_WINDOWS_REGISTERCLASS);
  }

  // Register the game window class.
  WNDCLASSEX wcGhost = {0};
  wcGame.style = CS_OWNDC;
  wcGhost.cbSize = sizeof(wcGhost);
  wcGhost.hInstance = hInstance;
  wcGhost.lpfnWndProc = GhostWindowProc;
  wcGhost.lpszClassName = GHOST_CLASS_NAME;

  if (!RegisterClassEx(&wcGhost)) {
    log_fatal("Error: Could not register Window Class \"%s\".",
              GHOST_CLASS_NAME);
    Win32HandleError(ERROR_PLATFORM_WINDOWS_REGISTERCLASS);
  }
}

LRESULT CALLBACK GhostWindowProc(HWND hwnd, UINT msg, WPARAM wParam,
                                 LPARAM lParam) {
  switch (msg) {
  case WM_CLOSE: {
    DestroyWindow(hwnd);
    return 0;
  }
  case WM_DESTROY: {
    PostQuitMessage(0);
    return 0;
  }
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
  case WM_CLOSE: {
    shouldClose = TRUE;
    DestroyWindow(hwnd);
    return 0;
  }
  case WM_DESTROY: {
    PostQuitMessage(0);
    return 0;
  }
  case WM_KEYDOWN:
  case WM_SYSKEYDOWN: {
    if (win32KeyStates == 0 || wParam > VK_RMENU ||
        (wParam > VK_F12 && wParam <= VK_NAVIGATION_CANCEL)) {
      log_error("Key: %u not supported.", wParam);
      break;
    }
    // get and set the key's state
    LT_INPUT_STATE state = (lParam & (1 << 30)) ? LT_KEY_DOWN : LT_KEY_PRESSED;
    win32KeyStates[wParam] = state;
    break;
  }
#ifdef LT_EDITOR
  // Handle drag window event
  case WM_NCHITTEST: {
    RECT wndRect;
    GetWindowRect(hwnd, &wndRect);
    int width = wndRect.right - wndRect.left;
    int height = wndRect.bottom - wndRect.top;

    // Get the location of the mouse click, which is packed into lParam.
    POINT pt;
    pt.x = LOWORD(lParam);
    pt.y = HIWORD(lParam);

    RECT rect;

    // Close button
    if (PtInRect(&rect, pt)) {
      return HTCLOSE;
    }
    const LRESULT result = DefWindowProc(hwnd, msg, wParam, lParam);

    SetRect(&rect, 3, 5, width - 3, 29);

    // Change Point coordinates to local window coords
    pt.x -= wndRect.left;
    pt.y -= wndRect.top;

    if ((result == HTCLIENT) && PtInRect(&rect, pt)) {
      return HTCAPTION;
    }
    return result;
  }
#endif
  default:
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  return 0;
}

void *Win32GetProc(const char *name) {
  void *proc = (void *)wglGetProcAddress(name);
  if (proc)
    return proc;

  proc = (void *)GetProcAddress(glInstance, name);

  if (proc == 0) {
    log_fatal("Retrieving %s failed.", name);
    Win32HandleError(ERROR_PLATFORM_OPENGL_PROC_NOT_FOUND);
  }

  return proc;
}

LoadProc Win32InitOpenGL(void) {
  glInstance = LoadLibraryA("opengl32.dll");
  if (glInstance == 0) {
    log_fatal("Couldn't load opengl library.");
    Win32HandleError(ERROR_PLATFORM_OPENGL_LIB_NOT_FOUND);
  }

  LT_Window_t ghostWnd;
  Win32_Helper_CreateWindow(&ghostWnd, GHOST_CLASS_NAME, CW_USEDEFAULT,
                            CW_USEDEFAULT, "");

  // IMPORTANT: before creating the contex pixel format must be set
  PIXELFORMATDESCRIPTOR pfd = {0};
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1; // Always set to 1
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 16;

  int pixelFormat = ChoosePixelFormat(ghostWnd.device, &pfd);
  if (pixelFormat == 0) {
    log_fatal("ChoosePixelFormat failed.");
    Win32HandleError(ERROR_PLATFORM_OPENGL_PIXELFORMAT);
  }

  if (SetPixelFormat(ghostWnd.device, pixelFormat, &pfd) == 0) {
    log_fatal("SetPixelFormat failed.");
    Win32HandleError(ERROR_PLATFORM_OPENGL_PIXELFORMAT);
  }

  // Create temporary legacy context
  HGLRC oldOGLcontext = wglCreateContext(ghostWnd.device);
  if (oldOGLcontext == NULL) {
    log_fatal("Create gl context failed.");
    Win32HandleError(ERROR_PLATFORM_OPENGL_CREATE_FAILED);
  }
  wglMakeCurrent(ghostWnd.device, oldOGLcontext);

  // Get GL extensions
  PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB =
      (PFNWGLCHOOSEPIXELFORMATARBPROC)Win32GetProc("wglChoosePixelFormatARB");
  PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
      (PFNWGLCREATECONTEXTATTRIBSARBPROC)Win32GetProc(
          "wglCreateContextAttribsARB");

  // Create modern context
  Win32_Helper_CreateWindow(&window, CLASS_NAME, 720, 480, WINDOW_TITLE);

  const int32 pixelAttribs[] = {WGL_DRAW_TO_WINDOW_ARB,
                                GL_TRUE,
                                WGL_SUPPORT_OPENGL_ARB,
                                GL_TRUE,
                                WGL_DOUBLE_BUFFER_ARB,
                                GL_TRUE,
                                WGL_PIXEL_TYPE_ARB,
                                WGL_TYPE_RGBA_ARB,
                                WGL_ACCELERATION_ARB,
                                WGL_FULL_ACCELERATION_ARB,
                                WGL_COLOR_BITS_ARB,
                                32,
                                WGL_ALPHA_BITS_ARB,
                                8,
                                WGL_DEPTH_BITS_ARB,
                                24,
                                WGL_STENCIL_BITS_ARB,
                                8,
                                WGL_SAMPLE_BUFFERS_ARB,
                                GL_TRUE,
                                WGL_SAMPLES_ARB,
                                4,
                                0};

  int32 pixelFormatID;
  uint32 numFormats;
  BOOL status = wglChoosePixelFormatARB(window.device, pixelAttribs, NULL, 1,
                                        &pixelFormatID, &numFormats);

  if (status == FALSE || numFormats == 0) {
    log_fatal("wglChoosePixelFormatARB() failed.");
    Win32HandleError(ERROR_PLATFORM_OPENGL_PIXELFORMAT);
  }

  PIXELFORMATDESCRIPTOR PFD;
  if (DescribePixelFormat(window.device, pixelFormatID, sizeof(PFD), &PFD) ==
      0) {
    log_fatal("Describe Modern PixelFormat failed.");
    Win32HandleError(ERROR_PLATFORM_OPENGL_PIXELFORMAT);
  }

  if (SetPixelFormat(window.device, pixelFormatID, &PFD) == 0) {
    log_fatal("Set Modern PixelFormat failed.");
    Win32HandleError(ERROR_PLATFORM_OPENGL_PIXELFORMAT);
  }

  const int major_min = 4;
  const int minor_min = 3;
  int contextAttribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,
                          major_min,
                          WGL_CONTEXT_MINOR_VERSION_ARB,
                          minor_min,
                          WGL_CONTEXT_PROFILE_MASK_ARB,
                          WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                          0};

  modernGLcontext =
      wglCreateContextAttribsARB(window.device, 0, contextAttribs);
  if (modernGLcontext == NULL) {
    log_fatal("Create modern gl context failed.");
    Win32HandleError(ERROR_PLATFORM_OPENGL_CREATE_MODERN_FAILED);
  }

  // Delete legacy context and ghost window
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(oldOGLcontext);
  ReleaseDC(ghostWnd.handle, ghostWnd.device);
  DestroyWindow(ghostWnd.handle);

  // Make the modern GL context the current
  wglMakeCurrent(window.device, modernGLcontext);
  ShowWindow(window.handle, SW_SHOW);
  log_info("Win32 OpenGL initialized.");
  return Win32GetProc;
}

noreturn void Win32HandleError(int32 in_exitCode) {
  LPTSTR msg;
  DWORD errorID = GetLastError();
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&msg, 0, NULL);

  log_fatal("%s", msg);
  LocalFree(msg);

  exit(in_exitCode);
}

#endif
