#pragma once
#include <Platform.h>
#ifdef LT_WINDOWS
#include <Windows.h>
// Window Procedures
LRESULT CALLBACK WindowProcGame(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Win32 helper functions
void Win32_Helper_RegisterWindowClasses(void);
void Win32_Helper_CreateWindow(Window* wnd, const char* in_wndClassName, int width, int height, const char* title);

// Exported functions
void Win32CreateWindow(int in_width, int in_height, const char* in_title);
void Win32InitOpenGL(void);
void Win32SwapBuffer(const Window* in_window);
#endif
