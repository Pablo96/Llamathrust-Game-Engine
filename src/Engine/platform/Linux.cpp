#include "Linux.hpp"

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>  // exit(code)
#include <time.h>

#include <ErrorCodes.hpp>
#include <Thread.hpp>
#include <cstring>
#include <log.hpp>
#include <stdexcept>

#include "../Engine.hpp"
#include "../Performance.hpp"

#ifdef LT_EDITOR
#define WINDOW_TITLE "Llamathrust GE (x64)"
#else
#define WINDOW_TITLE "Game LtGE (x64)"
#endif

// Windowing
LT::Window window;
static bool shouldClose = false;
#define WIDTH 720
#define HEIGHT 480

// Input
static uint64* xInputKeyStates = nullptr;
static uint64 xInputStatesSize = XK_Escape;

// Linux
static Display* display;
static Window xWindow;
static int32 screenId;
static Atom atomWmDeleteWindow;
static LT_NORETURN void LinuxHandleError(int32 in_exitCode);
static void inputMethodDestroyCallback(XIM im, XPointer clientData,
                                       XPointer callData) {}
static void inputContextDestroyCallback(XIC ic, XPointer clientData,
                                        XPointer callData) {}

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig,
                                                     GLXContext, Bool,
                                                     const int*);

static bool isExtensionSupported(const char* extList, const char* extension) {
  const char* start;
  const char *where, *terminator;

  where = strchr(extension, ' ');
  if (where || *extension == '\0') {
    return false;
  }

  for (start = extList;;) {
    where = strstr(start, extension);

    if (!where) {
      break;
    }

    terminator = where + strlen(extension);

    if (where == start || *(where - 1) == ' ') {
      if (*terminator == ' ' || *terminator == '\0') {
        return true;
      }
    }

    start = terminator;
  }

  return false;
}

#ifndef LT_NO_MAIN  // used for running tests
int main(int32 argc, const char** argv) {
  display = XOpenDisplay(nullptr);
  if (display == nullptr) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }
  screenId = DefaultScreen(display);

  // Opengl Context
  InitOpenGL();

  // Events
  static uint32 keyEventsMask = KeyPressMask | KeyReleaseMask | KeymapStateMask;
  static uint32 mouseEventsMask = PointerMotionMask | ButtonPressMask |
                                  ButtonReleaseMask | EnterWindowMask |
                                  LeaveWindowMask;
  static uint32 windowEventsMask = ExposureMask | StructureNotifyMask |
                                   FocusChangeMask | VisibilityChangeMask |
                                   PropertyChangeMask;

  // Suscribe to events
  XSelectInput(display, xWindow,
               keyEventsMask | mouseEventsMask | windowEventsMask);

  while (shouldClose == false) {
    LT_START_TIME();

    // Retrieve OS messages
    XPending(display);
    while (QLength(display)) {
      XEvent e;
      XNextEvent(display, &e);
      X11ProcEvent(&e);
    }
    XFlush(display);

    LT_END_TIME();
  }

  XCloseDisplay(display);
  log_info("LtGE: engine shutted down.");
  return 0;
}
#endif

//-------------------------------------------
// Window
//-------------------------------------------
void LT_CloseWindow(void) { shouldClose = true; }

void LinuxSwapBuffer(void) { glXSwapBuffers(display, xWindow); }

void X11ProcEvent(XEvent* event) {
  static char str[25];
  static KeySym keySym = 0;

  switch (event->type) {
    // SHOW WINDOW
    case Expose: {
      break;
    }
    case ClientMessage:
      if (event->xclient.data.l[0] != atomWmDeleteWindow) break;
    case DestroyNotify: {
      shouldClose = true;
      break;
    }
    // KEYS EVENTS
    case KeymapNotify: {
      XRefreshKeyboardMapping(&event->xmapping);
      break;
    }
    case KeyPress:
    case KeyRelease: {
      int32 len = XLookupString(&event->xkey, str, 25, &keySym, nullptr);
      if (len > 0) {
        log_error("key test %d", XK_KP_Divide);
        log_info("key pressed/released: %s - %d - %d", str, len, keySym);
      }
      if (keySym == XK_Escape) shouldClose = true;
      break;
    }
    // MOUSE EVENTS
    case ButtonPress: {
      if (event->xbutton.button == 1) {
        log_info("Left mouse\n");
      } else if (event->xbutton.button == 2) {
        log_info("Middle mouse down\n");
      } else if (event->xbutton.button == 3) {
        log_info("Right mouse down\n");
      } else if (event->xbutton.button == 4) {
        log_info("Mouse scroll up\n");
      } else if (event->xbutton.button == 5) {
        log_info("Mouse scroll down\n");
      }
      break;
    }
    case MotionNotify: {
      int32 x = event->xmotion.x;
      int32 y = event->xmotion.y;
      log_info("Mouse X: %d, Y: %d\n", x, y);
      break;
    }
    case EnterNotify: {
      log_info("Mouse Enter");
      break;
    }
    case LeaveNotify: {
      log_info("Mouse Leave");
      break;
    }
  }
}

//-------------------------------------------
// Graphics
//-------------------------------------------
LT::LoadProc InitOpenGL(void) {
  GLint majorGLX, minorGLX = 0;
  glXQueryVersion(display, &majorGLX, &minorGLX);
  if (majorGLX <= 1 && minorGLX < 2) {
    log_warn("GLX 1.2 or greater is required.");
    std::string msg = GET_ERROR_MSG(ERROR_PLATFORM_OPENGL_GLX_VERSION);
    log_fatal(msg.c_str());
    throw new std::runtime_error(msg);
  }

  log_info("GLX version: %d.%d", majorGLX, minorGLX);

  GLint glxAttribs[] = {GLX_X_RENDERABLE,
                        True,
                        GLX_DRAWABLE_TYPE,
                        GLX_WINDOW_BIT,
                        GLX_RENDER_TYPE,
                        GLX_RGBA_BIT,
                        GLX_X_VISUAL_TYPE,
                        GLX_TRUE_COLOR,
                        GLX_RED_SIZE,
                        8,
                        GLX_GREEN_SIZE,
                        8,
                        GLX_BLUE_SIZE,
                        8,
                        GLX_ALPHA_SIZE,
                        8,
                        GLX_DEPTH_SIZE,
                        24,
                        GLX_STENCIL_SIZE,
                        8,
                        GLX_DOUBLEBUFFER,
                        True,
                        None};

  int fbcount;
  GLXFBConfig* fbc =
      glXChooseFBConfig(display, DefaultScreen(display), glxAttribs, &fbcount);
  if (fbc == nullptr) {
    XCloseDisplay(display);
    std::string msg = GET_ERROR_MSG(ERROR_PLATFORM_OPENGL_CREATE_MODERN_FAILED);
    log_fatal(msg.c_str());
    throw new std::runtime_error(msg);
  }
  GLXFBConfig bestFbc = fbc[0];
  XFree(fbc);

  XVisualInfo* visual = glXGetVisualFromFBConfig(display, bestFbc);
  if (visual == nullptr) {
    XCloseDisplay(display);
    std::string msg = GET_ERROR_MSG(ERROR_PLATFORM_OPENGL_CREATE_FAILED);
    log_fatal(msg.c_str());
    throw new std::runtime_error(msg);
  }

  XSetWindowAttributes windowAttribs;
  windowAttribs.border_pixel = BlackPixel(display, screenId);
  windowAttribs.background_pixel = WhitePixel(display, screenId);
  windowAttribs.override_redirect = True;
  windowAttribs.colormap = XCreateColormap(
      display, RootWindow(display, screenId), visual->visual, AllocNone);
  windowAttribs.event_mask = ExposureMask | StructureNotifyMask |
                             FocusChangeMask | VisibilityChangeMask |
                             PropertyChangeMask;
  xWindow = XCreateWindow(
      display, RootWindow(display, screenId), 0, 0, 320, 200, 0, visual->depth,
      InputOutput, visual->visual,
      CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &windowAttribs);

  // Redirect close window
  atomWmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(display, xWindow, &atomWmDeleteWindow, 1);

  // Set title
  XStoreName(display, xWindow, WINDOW_TITLE);

  glXCreateContextAttribsARBProc glXCreateContextAttribsARB =
      (glXCreateContextAttribsARBProc)glXGetProcAddressARB(
          (const GLubyte*)"glXCreateContextAttribsARB");

  int context_attribs[] = {GLX_CONTEXT_MAJOR_VERSION_ARB,
                           3,
                           GLX_CONTEXT_MINOR_VERSION_ARB,
                           2,
                           GLX_CONTEXT_FLAGS_ARB,
                           GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                           None};

  const char* glxExts = glXQueryExtensionsString(display, screenId);
  GLXContext context = 0;
  if (!isExtensionSupported(glxExts, "GLX_ARB_create_context")) {
    context = glXCreateNewContext(display, bestFbc, GLX_RGBA_TYPE, 0, True);
  } else {
    context =
        glXCreateContextAttribsARB(display, bestFbc, 0, true, context_attribs);
  }
  XSync(display, False);

  // Verifying that context is a direct context
  if (!glXIsDirect(display, context)) {
    log_warn("Indirect GLX context obtained");
    XCloseDisplay(display);
    std::string msg = GET_ERROR_MSG(ERROR_PLATFORM_OPENGL_CONTEXT_INDIRECT);
    log_fatal(msg.c_str());
    throw new std::runtime_error(msg);
  }

  glXMakeCurrent(display, xWindow, context);

  log_info(
      "\nOPENGL info:\n"
      "\tVendor: %s\n"
      "\tRenderer: %s\n"
      "\tVersion: %s\n"
      "\tShading Language: %s",
      glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION),
      glGetString(GL_SHADING_LANGUAGE_VERSION));

  // Show window
  XMapWindow(display, xWindow);

  return nullptr;
}

LT::SwapBuffersFunc GetPlatformSwapBuffer(void) { return LinuxSwapBuffer; }

//-------------------------------------------
// Input
//-------------------------------------------
namespace LT {
void Platform::InitInput(int32* key_states) {
  xInputKeyStates = new uint64[xInputStatesSize];
  memset(xInputKeyStates, static_cast<int>(INPUT_STATE::LT_KEY_UP),
         xInputStatesSize);

  // Mouse Buttons
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_MOUSE_BUTTON_L)] = 1;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_MOUSE_BUTTON_R)] = 3;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_MOUSE_MIDDLE)] = 2;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_MOUSE_BUTTON_X1)] = 0;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_MOUSE_BUTTON_X2)] = 0;

  // Util Keys
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_BACKSPACE)] = XK_BackSpace;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_TAB)] = XK_Tab;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_DEL)] = XK_Clear;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_ENTER)] = XK_Return;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_MENU)] = XK_Menu;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_PAUSE)] = XK_Pause;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_ESCAPE)] = XK_Escape;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_SPACEBAR)] = XK_space;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_PAGE_UP)] = XK_Prior;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_PAGE_DOWN)] = XK_Next;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_END)] = XK_End;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_HOME)] = XK_Home;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_LEFT_ARROW)] = XK_Left;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_UP_ARROW)] = XK_Right;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_RIGHT_ARROW)] = XK_Up;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_DOWN_ARROW)] = XK_Down;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_SELECT)] = XK_Select;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_SCREENSHOT)] = XK_Sys_Req;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_EXECUTE)] = XK_Execute;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_PRINT_SCREEN)] = XK_Print;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_INSERT)] = XK_Insert;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_HELP)] = XK_Help;

  // Numers
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUM_0)] = XK_0;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUM_1)] = XK_1;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUM_2)] = XK_2;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUM_3)] = XK_3;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUM_4)] = XK_4;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUM_5)] = XK_5;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUM_6)] = XK_6;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUM_7)] = XK_7;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUM_8)] = XK_8;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUM_9)] = XK_9;

  // Letters
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_A)] = XK_A;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_B)] = XK_B;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_C)] = XK_C;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_D)] = XK_D;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_E)] = XK_E;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_F)] = XK_F;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_G)] = XK_G;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_H)] = XK_H;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_I)] = XK_I;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_J)] = XK_J;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_K)] = XK_K;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_L)] = XK_L;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_M)] = XK_M;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_N)] = XK_N;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_O)] = XK_O;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_P)] = XK_P;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_Q)] = XK_Q;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_R)] = XK_R;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_S)] = XK_S;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_T)] = XK_T;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_U)] = XK_U;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_V)] = XK_V;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_W)] = XK_W;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_X)] = XK_X;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_Y)] = XK_Y;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_Z)] = XK_Z;

  // NUM PAD
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUMPAD0)] = XK_KP_0;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUMPAD1)] = XK_KP_1;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUMPAD2)] = XK_KP_2;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUMPAD3)] = XK_KP_3;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUMPAD4)] = XK_KP_4;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUMPAD5)] = XK_KP_5;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUMPAD6)] = XK_KP_6;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUMPAD7)] = XK_KP_7;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUMPAD8)] = XK_KP_8;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUMPAD9)] = XK_KP_9;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_MULTIPLY)] = XK_KP_Multiply;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_PLUS)] = XK_KP_Add;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_SEPARATOR)] = XK_KP_Separator;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_SUBTRACT)] = XK_KP_Subtract;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUM_DOT)] = XK_KP_Decimal;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_DIVIDE)] = XK_KP_Divide;

  // Functions keys
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_F1)] = XK_F1;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_F2)] = XK_F2;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_F3)] = XK_F3;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_F4)] = XK_F4;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_F5)] = XK_F5;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_F6)] = XK_F6;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_F7)] = XK_F7;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_F8)] = XK_F8;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_F9)] = XK_F9;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_F10)] = XK_F10;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_F11)] = XK_F11;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_F12)] = XK_F12;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_CAPS_LOCK)] = XK_Caps_Lock;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_NUM_LOCK)] = XK_Num_Lock;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_SCROLL_LOCK)] =
      XK_Scroll_Lock;

  // Mods
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_SUPER_L)] = XK_Super_L;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_SUPER_R)] = XK_Super_R;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_SHIFT_L)] = XK_Shift_L;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_SHIFT_R)] = XK_Shift_R;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_ALT_L)] = XK_Alt_L;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_ALT_R)] = XK_Alt_R;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_CONTROL_L)] = XK_Control_L;
  key_states[static_cast<int64>(INPUT_KEY::LT_KEY_CONTROL_R)] = XK_Control_R;
}

uint8 Platform::GetKeyState(int32 keyState) { return 0; }
}  // namespace LT
LT::INPUT_KEY X11TranslateKeys(uint64* key) {
  log_info("KeyCode %p", key);
  return LT::INPUT_KEY::KEYS_COUNT;
}
//-------------------------------------------
// Shared libs
//-------------------------------------------
namespace LT {
void* Platform::LoadSharedLib(const char* name) {
  return dlopen(name, RTLD_LAZY);
}

void* Platform::GetProc(void* in_lib, const char* in_name) {
  return dlsym(in_lib, in_name);
}
}  // namespace LT

//-------------------------------------------
// Threads
//-------------------------------------------
namespace LT {
LT::Thread* Platform::ThreadCreate(LT::Thread* thread,
                                   LT::ThreadFuncWrapper funcWrapper) {
  pthread_t threadID;
  void* threadArg = nullptr;
  int32 result = pthread_create(&threadID, nullptr, funcWrapper, threadArg);

  if (result != 0) {
    log_error("Failed to create thread.");
    LinuxHandleError(ERROR_PLATFORM_THREAD_CREATE);
  }

  LT::ThreadLinux nixThd = LT::ThreadLinux(threadID);
  std::memcpy(thread->reserved, &nixThd, sizeof(LT::ThreadLinux));
  return thread;
}

void Platform::ThreadStart(const LT::Thread* thread) {
  // Does nothing
}

void Platform::GetCurrent(const LT::Thread* thread) {
  pthread_t threadID = pthread_self();
  LT::ThreadLinux nixThd = LT::ThreadLinux(threadID);
  std::memcpy(
      const_cast<void*>(reinterpret_cast<const void*>(thread->reserved)),
      &nixThd, sizeof(LT::ThreadLinux));
}

void Platform::ThreadJoin(const LT::Thread* thread) {
  pthread_join(
      reinterpret_cast<uint64>(thread->reserved),
      const_cast<void**>(reinterpret_cast<void* const*>(&thread->exitCode)));
}

void Platform::ThreadSleep(const LT::Thread* thread, const uint64 miliseconds) {
  struct timespec ts;
  int res;

  if (miliseconds < 0) return;

  ts.tv_sec = miliseconds / 1000;
  ts.tv_nsec = (miliseconds % 1000) * 1000000;

  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);
}

LT_NORETURN void Platform::ThreadExit(const int32 exit_code) {
  void* exitCode = reinterpret_cast<void*>(exit_code);
  pthread_exit(exitCode);
}

void Platform::ThreadGetExitCode(LT::Thread* thread) {
  if (!pthread_tryjoin_np(reinterpret_cast<uint64>(thread->reserved),
                          reinterpret_cast<void**>(&thread->exitCode))) {
    log_error("Thread still active");
  } else {
    thread->isValid = false;
  }
}

void Platform::ThreadDestroy(LT::Thread* thread) { thread->isValid = false; }
}  // namespace LT

//-------------------------------------------
// Threading Locks
//-------------------------------------------
namespace LT {
LT::ThreadLock* Platform::ThreadLockCreate() {
  pthread_mutex_t* lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(lock, nullptr);
  return reinterpret_cast<LT::ThreadLock*>(lock);
}

void Platform::ThreadLockLock(LT::ThreadLock* lock) {
  pthread_mutex_lock(reinterpret_cast<pthread_mutex_t*>(lock));
}

void Platform::ThreadLockUnock(LT::ThreadLock* lock) {
  pthread_mutex_unlock(reinterpret_cast<pthread_mutex_t*>(lock));
}

void Platform::ThreadLockDestroy(LT::ThreadLock* lock) {
  pthread_mutex_destroy(reinterpret_cast<pthread_mutex_t*>(lock));
}
}  // namespace LT

//-------------------------------------------
// Networking
//-------------------------------------------

namespace LT {
void Platform::NetAddressCreate(LT::NetAddress* address) {}

void Platform::NetAddressDestroy(LT::NetAddress* address) {}

void Platform::SocketCreate(LT::NetSocket* socket) {}

bool Platform::SocketBind(const LT::NetSocket* socket) { return false; }

bool Platform::SocketListen(const LT::NetSocket* socket) { return false; }

LT::NetSocket* Platform::SocketAccept(const LT::NetSocket* in_socket) {
  return nullptr;
}

void Platform::SocketClose(LT::NetSocket* socket) {}

bool Platform::SocketConnect(LT::NetSocket* socket,
                             const LT::NetAddress* address) {
  return false;
}

bool Platform::SocketConnClose(const LT::NetSocket* socket) { return false; }

bool Platform::SocketSend(const LT::NetSocket* socket, const char* msg,
                          const uint32 msg_len) {
  return false;
}

bool Platform::SocketRecieve(const LT::NetSocket* socket, char* msg,
                             uint32 buffer_size, uint32* msg_len) {
  return false;
}
}  // namespace LT

static LT_NORETURN void LinuxHandleError(int32 in_exitCode) {
  exit(in_exitCode);
}
