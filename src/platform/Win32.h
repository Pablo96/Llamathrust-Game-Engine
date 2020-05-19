#include <Platform.h>
#ifdef LT_WINDOWS
#include <Windows.h> 

LRESULT CALLBACK WindowProcGame(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND Win32CreateWindow(HINSTANCE hInstance, const char* in_wndClassName, int width, int height, const char* title);
void PlatformCreateWindow(int in_width, int in_height, const char* in_title);
#endif
