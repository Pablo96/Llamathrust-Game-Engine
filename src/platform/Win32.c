#include "Win32.h"
#include "../Engine.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef LT_WINDOWS
#include <Windows.h>

int main(int argc, const char** argv)
{
#if defined(LT_DEBUG)
    printf("Debug Version\n");
#elif defined(LT_RELEASE)
    printf("Release Version\n");
#else
    printf("Other Version\n");
#endif
    printf("Running program: %s\n\n", argv[0]);
    
    // Try to open the mutex.
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, "LlamathrustMutex");

    // If mutex doesnt exists create it and run the engine
    if (!hMutex)
      hMutex = CreateMutex(0, 0, "LlamathrustMutex");
    // Else there is an instance of the engine running
    else {
        printf("Instance already running\n");
        return 48;
    }

    // Get handle to this executable?
    HINSTANCE hInstance = GetModuleHandle(NULL);
    
    // Register the window classes.
    const char CLASS_NAME[]  = "GameWindow";
    WNDCLASS wc = {0};
    wc.lpfnWndProc   = WindowProcGame;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);
    printf("Window class \"%s\" registered.\n", CLASS_NAME);

    // Create main editor window or game window
    HWND wndHandle = Win32CreateWindow(hInstance, CLASS_NAME, 720, 480, "Game x64 (llamathrust) [clang]");
    ShowWindow(wndHandle, SW_SHOW);


    // Set platform functions pointers
    create_window = PlatformCreateWindow;

    // Start the engine
    Engine_Start();

    // Main engine loop
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
    
    // Clean up the engine resources if needed
    Engine_Shutdown();

    return 0;
}

void PlatformCreateWindow(int in_width, int in_height, const char* in_title)
{
    HINSTANCE hInstance = GetModuleHandleW(NULL);
    HWND wndHandle = Win32CreateWindow(hInstance, "EditorWindow", in_width, in_height, in_title);
    ShowWindow(wndHandle, SW_SHOW);
}

HWND Win32CreateWindow(HINSTANCE hInstance, const char* in_wndClassName, int width, int height, const char* title)
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
        printf("Error creating window of class \"%s\".\n", in_wndClassName);
        exit(1);
    }

    printf("Window of class \"%s\" created.\n", in_wndClassName);
    return hwnd;
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
