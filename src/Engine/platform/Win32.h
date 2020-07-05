#pragma once
#include <Platform.h>
#ifdef LT_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

typedef void *(*Win32loadproc)(const char *name);

// Window Procedures
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GhostWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                 LPARAM lParam);

// Win32 helper functions
void *Win32GetProc(const char *name);
void Win32_Helper_RegisterWindowClasses(void);
void Win32_Helper_InitNetworking(void);
void Win32_Helper_CreateWindow(Window *wnd, const char *in_wndClassName,
                               int width, int height, const char *title);

// Exported functions
LoadProc Win32InitOpenGL(void);
void Win32SwapBuffer(void);
Win32loadproc Win32InitOpenGL(void);
#endif
