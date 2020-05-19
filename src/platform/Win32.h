#include <Platform.h>
#ifdef LT_WINDOWS
#include <Windows.h> 

static const char* GAME_CLASS_NAME = "GameWindow";
static const char* EDITOR_CLASS_NAME = "EditorWindow";
static HINSTANCE hInstance;

LRESULT CALLBACK WindowProcGame(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Win32RegisterWindowClasses(void);
HWND Win32CreateWindow(const char* in_wndClassName, int width, int height, const char* title);
void PlatformCreateWindow(int in_width, int in_height, const char* in_title);
#endif
