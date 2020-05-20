#include "Win32.h"
#ifdef LT_WINDOWS
#include "../Engine.h"
#include <log.h>
#include <stdlib.h>
#include <Windows.h>
#include <wingdi.h>
#include <stdnoreturn.h>

// Windows
#ifdef LT_EDITOR
static uint32 windowsMaxCount = 8;
#else   // if it is a game only 1 window is allowed
static uint32 windowsMaxCount = 1;
#endif

uint32 windowsCount = 0;
Window* windowsVec;
void (*LT_CreateWindow)(int32, int32, const char*);

// Win32
static HINSTANCE hInstance;
static const LPTSTR EDITOR_CLASS_NAME = "EditorWindow";
static const LPTSTR GAME_CLASS_NAME = "GameWindow";
static const LPTSTR GHOST_CLASS_NAME = "GhostWindow";
noreturn static void Win32HandleError(int32 in_exitCode);

int main(int32 argc, const char** argv)
{
    // Get handle to this executable
    hInstance = GetModuleHandle(NULL);

    //-----------------------------------------------------------------
    // Parse command line arguments
    //-----------------------------------------------------------------
    if (argc > 1) {
        for (int32 i = 0; i < argc; i++)
            log_trace("Command line argument: %s", argv[i]);
        log_info("Command line arguments parsed!.\n");
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
        return 48;
    }

    //-----------------------------------------------------------------
    // Start setting app the platform layer
    //-----------------------------------------------------------------
    Win32_Helper_RegisterWindowClasses();

    // Set platform functions pointers
    LT_CreateWindow = Win32CreateWindow;

    //-----------------------------------------------------------------
    // Start the engine
    //-----------------------------------------------------------------s
    Engine_Start();

    //-----------------------------------------------------------------
    // Main engine loop
    //-----------------------------------------------------------------
    MSG msg = {0};
    BOOL shouldClose = 0;
    while(!shouldClose) {
        // Run engine
        Engine_Run(1.0f / 60.0f);

        // Retrieve OS messages
        while(PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT && windowsCount == 0) {
                shouldClose = 1;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    //-----------------------------------------------------------------
    // Cleaning at exit
    //-----------------------------------------------------------------
    // Clean up the engine resources if needed
    Engine_Shutdown();

    return 0;
}

void Win32InitOpenGL(void) {
    Window* ghostWnd = Win32_Helper_CreateWindow(GHOST_CLASS_NAME, CW_USEDEFAULT, CW_USEDEFAULT, "");

    // IMPORTANT: before creating the contex pixel format must be set
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize       = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion    = 1;   // Always set to 1
    pfd.dwFlags     = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    pfd.iPixelType  = PFD_TYPE_RGBA;
    pfd.cColorBits  = 32;
    pfd.cDepthBits  = 16;
    
    int pixelFormat = ChoosePixelFormat(ghostWnd->device, &pfd);
    if (pixelFormat == 0) {
        log_fatal("ChoosePixelFormat failed.");
        Win32HandleError(50);
    }

    if (SetPixelFormat(ghostWnd->device, pixelFormat, &pfd) != TRUE) {
        log_fatal("SetPixelFormat failed.");
        Win32HandleError(50);
    }

  
    DestroyWindow(ghostWnd->handle);

    log_info("Win32 OpenGL initialized.");
}

void Win32SwapBuffer(const Window* in_window) {
    SwapBuffers(in_window->device);
}

void Win32CreateWindow(int in_width, int in_height, const char* in_title)
{
    if (windowsCount == windowsMaxCount) {
        log_error("Max windows count reached!");
        return;
    }

    Window* wnd = Win32_Helper_CreateWindow(GAME_CLASS_NAME, in_width, in_height, in_title);
    ShowWindow(wnd->handle, SW_SHOW);
}

Window* Win32_Helper_CreateWindow(const char* in_wndClassName, int width, int height, const char* title)
{
    DWORD styleEx   = in_wndClassName == GAME_CLASS_NAME
                    ? WS_OVERLAPPED
                    : WS_DISABLED;

    HWND hwnd = CreateWindowEx(
    styleEx,                       // Optional window styles.
    in_wndClassName,                // Window class
    title,                          // Window text
    WS_OVERLAPPEDWINDOW,            // Window style
    CW_USEDEFAULT, CW_USEDEFAULT,   // Window position
    width, height,                  // Window size
    NULL,                           // Parent window    
    NULL,                           // Menu
    hInstance,                      // Instance handle
    NULL                            // Additional application data
    );

    if (hwnd == NULL)
    {
        log_fatal("Error creating window of class \"%s\".", in_wndClassName);
        Win32HandleError(1);
    }

    // save the window in the vector
    if (windowsVec == NULL) {
        windowsVec = malloc(sizeof(Window) * windowsMaxCount);
    }

    windowsVec[windowsCount].handle = hwnd;
    windowsVec[windowsCount].device = GetDC(hwnd);
    windowsCount++;

    log_info("Window of class \"%s\" created.", in_wndClassName);
    return &windowsVec[windowsCount - 1];
}

void Win32_Helper_RegisterWindowClasses() {
    // Register the game window class.
    WNDCLASSEX wcGame = {0};
    wcGame.cbSize        = sizeof(wcGame);
    wcGame.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcGame.hInstance     = hInstance;
    wcGame.lpfnWndProc   = WindowProcGame;
    wcGame.lpszClassName = GAME_CLASS_NAME;

    if (!RegisterClassEx(&wcGame)) {
        log_fatal("Error: Could not register Window Class \"%s\".", GAME_CLASS_NAME);
        Win32HandleError(49);
    }
    log_info("Window class \"%s\" registered.", GAME_CLASS_NAME);

    // Register the game window class.
    WNDCLASSEX wcGhost = {0};
    wcGhost.cbSize        = sizeof(wcGhost);
    wcGhost.hInstance     = hInstance;
    wcGhost.lpfnWndProc   = WindowProcGame;
    wcGhost.lpszClassName = GHOST_CLASS_NAME;

    if (!RegisterClassEx(&wcGhost)) {
        log_fatal("Error: Could not register Window Class \"%s\".", GHOST_CLASS_NAME);
        Win32HandleError(49);
    }

    log_info("Window class \"%s\" registered.", GHOST_CLASS_NAME);
}

LRESULT CALLBACK WindowProcGame(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg) {
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            return 0;
        }
        case WM_DESTROY:
        {
            for (uint32 i = 0; i < windowsCount; i++) {
                if (windowsVec[i].handle == hwnd) {
                    if (i < 1)
                        windowsVec = windowsVec + 1;
                    else
                        windowsVec[i] = windowsVec[windowsCount - 1];
                }
            }
            windowsCount--;
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

noreturn void Win32HandleError(int32 in_exitCode) {
    LPTSTR msg;
    DWORD errorID = GetLastError();
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorID,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &msg,
        0, NULL
    );

    log_fatal("%s", msg);
    LocalFree(msg);

    exit(in_exitCode);
}
#endif
