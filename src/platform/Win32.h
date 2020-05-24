#pragma once
#include <Platform.h>
#ifdef LT_WINDOWS
#include <Windows.h>
// Window Procedures
LRESULT CALLBACK WindowProcGame(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Win32 helper functions
void Win32_Helper_RegisterWindowClasses(void);
void Win32_Helper_CreateWindow(Window* wnd, const char* in_wndClassName, int width, int height, const char* title);
void* Win32GetProc(const char* name);

// Exported functions
LoadProc Win32InitOpenGL(void);
void Win32SwapBuffer(void);
#endif
