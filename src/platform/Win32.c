#include "Win32.h"
#ifdef LT_WINDOWS
#include "../Engine.h"
#include <Windows.h>
#include <gl/GL.h>
#include <gl/wglext.h>
#include <log.h>
#include <stdlib.h>
#include <wingdi.h>

#ifdef __clang__
#include <stdnoreturn.h>
#elif defined(_MSC_VER)
#define noreturn
#endif

// Windows
Window window;

static BOOL shouldClose = FALSE;

// Win32
static HINSTANCE hInstance;
static HINSTANCE glInstance;
static HGLRC modernGLcontext;
#ifdef LT_EDITOR
static const LPTSTR CLASS_NAME = "EditorWindow";
#define CLASS_STYLE WS_OVERLAPPED
#define WindowProcedure WindowProcEditor
#else
static const LPTSTR CLASS_NAME = "GameWindow";
#define CLASS_STYLE WS_OVERLAPPED
#define WindowProcedure WindowProcGame
#endif
static const LPTSTR GHOST_CLASS_NAME = "GhostWindow";
noreturn static void Win32HandleError(int32 in_exitCode);

int main(int32 argc, const char **argv) {
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

  //-----------------------------------------------------------------
  // Start the engine
  //-----------------------------------------------------------------
  Engine_Start();

  //-----------------------------------------------------------------
  // Main engine loop
  //-----------------------------------------------------------------
  MSG msg = {0};
  while (shouldClose == FALSE) {
    // Run engine
    Engine_Run(1.0f / 60.0f);

    // Retrieve OS messages
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
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

void *Win32GetProc(const char *name) {
  void *proc = (void *)wglGetProcAddress(name);
  if (proc)
    return proc;

  proc = (void *)GetProcAddress(glInstance, name);

  if (proc == 0) {
    log_fatal("Retrieving %s failed.", name);
    Win32HandleError(50);
  }

  return proc;
}

Win32loadproc Win32InitOpenGL(void) {
  glInstance = LoadLibraryA("opengl32.dll");
  if (glInstance == 0) {
    log_fatal("Couldn't load opengl library.");
    Win32HandleError(50);
  }

  Window ghostWnd;
  Win32_Helper_CreateWindow(&ghostWnd, GHOST_CLASS_NAME, CW_USEDEFAULT,
                            CW_USEDEFAULT, "");

  // IMPORTANT: before creating the contex pixel format must be set
  PIXELFORMATDESCRIPTOR pfd = {0};
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1; // Always set to 1
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 16;

  int pixelFormat = ChoosePixelFormat(ghostWnd.device, &pfd);
  if (pixelFormat == 0) {
    log_fatal("ChoosePixelFormat failed.");
    Win32HandleError(50);
  }

  if (SetPixelFormat(ghostWnd.device, pixelFormat, &pfd) == 0) {
    log_fatal("SetPixelFormat failed.");
    Win32HandleError(50);
  }

  // Create temporary legacy context
  HGLRC oldOGLcontext = wglCreateContext(ghostWnd.device);
  if (oldOGLcontext == NULL) {
    log_fatal("Create modern gl context failed.");
    Win32HandleError(50);
  }
  wglMakeCurrent(ghostWnd.device, oldOGLcontext);

  // Get GL extensions
  PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB =
      (PFNWGLCHOOSEPIXELFORMATARBPROC)Win32GetProc("wglChoosePixelFormatARB");
  PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
      (PFNWGLCREATECONTEXTATTRIBSARBPROC)Win32GetProc(
          "wglCreateContextAttribsARB");

  // Create modern context
  Win32_Helper_CreateWindow(&window, CLASS_NAME, 720, 480, "Game x64");

  const int32 pixelAttribs[] = {WGL_DRAW_TO_WINDOW_ARB,
                                GL_TRUE,
                                WGL_SUPPORT_OPENGL_ARB,
                                GL_TRUE,
                                WGL_DOUBLE_BUFFER_ARB,
                                GL_TRUE,
                                WGL_PIXEL_TYPE_ARB,
                                WGL_TYPE_RGBA_ARB,
                                WGL_ACCELERATION_ARB,
                                WGL_FULL_ACCELERATION_ARB,
                                WGL_COLOR_BITS_ARB,
                                32,
                                WGL_ALPHA_BITS_ARB,
                                8,
                                WGL_DEPTH_BITS_ARB,
                                24,
                                WGL_STENCIL_BITS_ARB,
                                8,
                                WGL_SAMPLE_BUFFERS_ARB,
                                GL_TRUE,
                                WGL_SAMPLES_ARB,
                                4,
                                0};

  int32 pixelFormatID;
  uint32 numFormats;
  BOOL status = wglChoosePixelFormatARB(window.device, pixelAttribs, NULL, 1,
                                        &pixelFormatID, &numFormats);

  if (status == FALSE || numFormats == 0) {
    log_fatal("wglChoosePixelFormatARB() failed.");
    Win32HandleError(50);
  }

  PIXELFORMATDESCRIPTOR PFD;
  if (DescribePixelFormat(window.device, pixelFormatID, sizeof(PFD), &PFD) ==
      0) {
    log_fatal("Describe Modern PixelFormat failed.");
    Win32HandleError(50);
  }

  if (SetPixelFormat(window.device, pixelFormatID, &PFD) == 0) {
    log_fatal("Set Modern PixelFormat failed.");
    Win32HandleError(50);
  }

  const int major_min = 4;
  const int minor_min = 3;
  int contextAttribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,
                          major_min,
                          WGL_CONTEXT_MINOR_VERSION_ARB,
                          minor_min,
                          WGL_CONTEXT_PROFILE_MASK_ARB,
                          WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                          0};

  modernGLcontext =
      wglCreateContextAttribsARB(window.device, 0, contextAttribs);
  if (modernGLcontext == NULL) {
    log_fatal("Create modern gl context failed.");
    Win32HandleError(50);
  }

  // Delete legacy context and ghost window
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(oldOGLcontext);
  ReleaseDC(ghostWnd.handle, ghostWnd.device);
  DestroyWindow(ghostWnd.handle);

  // Make the modern GL context the current
  wglMakeCurrent(window.device, modernGLcontext);
  ShowWindow(window.handle, SW_SHOW);
  log_info("Win32 OpenGL initialized.");
  return Win32GetProc;
}

void Win32SwapBuffer() { SwapBuffers(window.device); }

void Win32_Helper_CreateWindow(Window *wnd, const char *in_wndClassName,
                               int width, int height, const char *title) {
  DWORD styleEx = in_wndClassName == CLASS_NAME ? CLASS_STYLE : WS_DISABLED;

  HWND hwnd = CreateWindowEx(styleEx,             // Optional window styles.
                             in_wndClassName,     // Window class
                             title,               // Window text
                             WS_OVERLAPPEDWINDOW, // Window style
                             CW_USEDEFAULT, CW_USEDEFAULT, // Window position
                             width, height,                // Window size
                             NULL,                         // Parent window
                             NULL,                         // Menu
                             hInstance,                    // Instance handle
                             NULL // Additional application data
  );

  if (hwnd == NULL) {
    log_fatal("Error creating window of class \"%s\".", in_wndClassName);
    Win32HandleError(1);
  }

  // save the window in the vector
  wnd->handle = hwnd;
  wnd->device = GetDC(hwnd);

  log_info("Window of class \"%s\" created.", in_wndClassName);
}

void Win32_Helper_RegisterWindowClasses() {
  // Register the game window class.
  WNDCLASSEX wcGame = {0};
  wcGame.cbSize = sizeof(wcGame);
  wcGame.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wcGame.hInstance = hInstance;
  wcGame.lpfnWndProc = WindowProc;
  wcGame.lpszClassName = CLASS_NAME;

  if (!RegisterClassEx(&wcGame)) {
    log_fatal("Error: Could not register Window Class \"%s\".", CLASS_NAME);
    Win32HandleError(49);
  }
  log_info("Window class \"%s\" registered.", CLASS_NAME);

  // Register the game window class.
  WNDCLASSEX wcGhost = {0};
  wcGame.style = CS_OWNDC;
  wcGhost.cbSize = sizeof(wcGhost);
  wcGhost.hInstance = hInstance;
  wcGhost.lpfnWndProc = GhostWindowProc;
  wcGhost.lpszClassName = GHOST_CLASS_NAME;

  if (!RegisterClassEx(&wcGhost)) {
    log_fatal("Error: Could not register Window Class \"%s\".",
              GHOST_CLASS_NAME);
    Win32HandleError(49);
  }

  log_info("Window class \"%s\" registered.", GHOST_CLASS_NAME);
}

LRESULT CALLBACK GhostWindowProc(HWND hwnd, UINT msg, WPARAM wParam,
                                 LPARAM lParam) {
  switch (msg) {
  case WM_CLOSE: {
    DestroyWindow(hwnd);
    return 0;
  }
  case WM_DESTROY: {
    PostQuitMessage(0);
    return 0;
  }
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
  case WM_CREATE: {
    shouldClose = FALSE;
    return 0;
  }
  case WM_CLOSE: {
    shouldClose = TRUE;
    DestroyWindow(hwnd);
    return 0;
  }
  case WM_DESTROY: {
    PostQuitMessage(0);
    return 0;
  }
#ifdef LT_EDITOR

#endif
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

noreturn void Win32HandleError(int32 in_exitCode) {
  LPTSTR msg;
  DWORD errorID = GetLastError();
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&msg, 0, NULL);

  log_fatal("%s", msg);
  LocalFree(msg);

  exit(in_exitCode);
}
#endif
