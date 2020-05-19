#include "Win32.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef LT_WINDOWS
#include <Windows.h>

int main(int argc, const char** argv)
{
    printf("Running program: %s\n\n", argv[0]);
    
    HINSTANCE hInstance = GetModuleHandleW(NULL);
    
    // Register the window class.
    const char CLASS_NAME[]  = "GameWindow";
    WNDCLASS wc = {0};
    wc.lpfnWndProc   = WindowProcGame;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);
    printf("Window class \"%s\" registered.\n", CLASS_NAME);

    HWND wndHandle = Win32CreateWindow(hInstance, CLASS_NAME);
    ShowWindow(wndHandle, SW_SHOW);

    MSG msg = {0};
    BOOL shouldClose = 0;
    while(!shouldClose) {
        // Run engine

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

    return 0;
}

HWND Win32CreateWindow(HINSTANCE hInstance, const char* in_wndClassName)
{
    HWND hwnd = CreateWindowEx(
    0,                              // Optional window styles.
    in_wndClassName,                     // Window class
    "Game x64 (llamathrust) [clang]",    // Window text
    WS_OVERLAPPEDWINDOW,            // Window style

    // Size and position
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

    NULL,       // Parent window    
    NULL,       // Menu
    hInstance,  // Instance handle
    NULL        // Additional application data
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
