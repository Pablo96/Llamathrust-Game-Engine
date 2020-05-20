#include "Win32.h"

#ifdef LT_WINDOWS
#include "../Engine.h"
#include <log.h>
#include <stdlib.h>
#include <Windows.h>

typedef struct _Window {
    HWND handle;
    HDC device;
} Window;

// Windows
static uint32 windowMaxCount = 4;
uint32 windowsCount = 0;
Window* windowsVec;

// Win32
static const char* GAME_CLASS_NAME = "GameWindow";
static const char* EDITOR_CLASS_NAME = "EditorWindow";
static HINSTANCE hInstance;

int main(int argc, const char** argv)
{
    // Get handle to this executable
    hInstance = GetModuleHandle(NULL);

    //-----------------------------------------------------------------
    // Parse command line arguments
    //-----------------------------------------------------------------
    if (argc > 1) {
#ifdef LT_DEBUG
        log_info("Command line arguments parsed!.\n");
#endif
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

    // Create main editor window or game window
    HWND wndHandle = Win32_Helper_CreateWindow(GAME_CLASS_NAME, 720, 480, "Game x64 (llamathrust) [clang]");
    ShowWindow(wndHandle, SW_SHOW);

    // Set platform functions pointers
    create_window = Win32CreateWindow;

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
            if (msg.message == WM_QUIT) {
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

void Win32SwapBuffer(const Window* in_window) {
    SwapBuffers(in_window->device);
}

void Win32CreateWindow(int in_width, int in_height, const char* in_title)
{
    HWND wndHandle = Win32_Helper_CreateWindow(EDITOR_CLASS_NAME, in_width, in_height, in_title);
    ShowWindow(wndHandle, SW_SHOW);
}

HWND Win32_Helper_CreateWindow(const char* in_wndClassName, int width, int height, const char* title)
{
    HWND hwnd = CreateWindowEx(
    0,                              // Optional window styles.
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
        exit(1);
    }

    // save the window in the vector
    if (windowsVec == NULL) {
        windowsVec = malloc(sizeof(Window) * windowMaxCount);
    }

    windowsVec[windowsCount].handle = hwnd;
    windowsVec[windowsCount].device = GetDC(hwnd);
    windowsCount++;

    log_info("Window of class \"%s\" created.", in_wndClassName);
    return hwnd;
}

void Win32_Helper_RegisterWindowClasses() {
    // Register the game window class.
    WNDCLASS wc = {0};
    wc.lpfnWndProc   = WindowProcGame;
    wc.hInstance     = hInstance;
    wc.lpszClassName = GAME_CLASS_NAME;

    if (!RegisterClass(&wc)) {
        log_fatal("Error: Could not register Window Class \"%s\".", GAME_CLASS_NAME);
    }

#ifdef LT_DEBUG
    log_info("Window class \"%s\" registered.", GAME_CLASS_NAME);
#endif
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
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

#endif
