#include <Platform.h>
#ifdef LT_WINDOWS
#include <Windows.h> 

LRESULT CALLBACK WindowProcGame(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND Win32CreateWindow(HINSTANCE hInstance, const char* in_wndClassName);

#endif
