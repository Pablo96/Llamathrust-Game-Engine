#include "Linux.hpp"
#include <log.hpp>
#include <Thread.hpp>
#include <ErrorCodes.hpp>
#include "../Performance.hpp"
#include "../Engine.hpp"

#include <dlfcn.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <stdlib.h> // exit(code)
#include <cstring>

// Windowing
LT::Window window;
static bool shouldClose = false;
#define WIDTH 720
#define HEIGHT 480

// Linux
static Display *display;
static LT_NORETURN void LinuxHandleError(int32 in_exitCode);
static void inputMethodDestroyCallback(XIM im, XPointer clientData, XPointer callData) {}
static void inputContextDestroyCallback(XIC ic, XPointer clientData, XPointer callData) {}

#ifndef LT_NO_MAIN //used for running tests
int main(int32 argc, const char **argv) {
  display = XOpenDisplay(nullptr);
  if (display == nullptr) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }


  int screen = DefaultScreen(display);
  int depth = DefaultDepth(display, screen);
  Visual* visual = DefaultVisual(display, screen);
  Window root = RootWindow(display, screen);
  XSetWindowAttributes windowAttributes = { 0 };
  
  // Create a colormap based on the visual used by the current context
  windowAttributes.colormap = XCreateColormap(display, root, visual, AllocNone);
  windowAttributes.your_event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                                PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
                                ExposureMask | FocusChangeMask | VisibilityChangeMask |
                                EnterWindowMask | LeaveWindowMask | PropertyChangeMask;
  
  Window xWindow = XCreateWindow(
    display,        // server connection
    root,           // root window
    0, 0,           // position
    WIDTH, HEIGHT,  // size
    0,              // border size
    depth,          // color depth (bits per pixel)
    InputOutput,
    visual,
    CWBorderPixel | CWColormap | CWEventMask,
    &windowAttributes);
  
  // Show window
  XMapWindow(display, xWindow);

  // Input setup
  XIM im = XOpenIM(display, 0, nullptr, nullptr);
  if (!im)
    return 0;
  
  XIMCallback callback;
  callback.callback = inputMethodDestroyCallback;
  callback.client_data = nullptr;
  XSetIMValues(im, XNDestroyCallback, &callback, nullptr);

  callback.callback = reinterpret_cast<XIMProc>(inputContextDestroyCallback);
  callback.client_data = reinterpret_cast<XPointer>(&window);
  XIC ic = XCreateIC(im,
                    XNInputStyle,
                    XIMPreeditNothing | XIMStatusNothing,
                    XNClientWindow,
                    xWindow,
                    XNFocusWindow,
                    xWindow,
                    XNDestroyCallback,
                    &callback,
                    nullptr);

  if (!im)
    return 0;
  
  unsigned long filter = 0;
  if (XGetICValues(ic, XNFilterEvents, &filter, nullptr) == nullptr)
  {
      XSelectInput(display,
                  xWindow,
                  windowAttributes.your_event_mask | filter);
  }


  // Opengl Context

  while (shouldClose == false) {
    LT_START_TIME();

    // Retrieve OS messages
    XPending(display);
    while (QLength(display))
    {
      XEvent e;
      XNextEvent(display, &e);
      X11ProcEvent(xWindow, &e, screen);
    }
    XFlush(display);

    LT_END_TIME();
  }

  XCloseDisplay(display);
  return 0;
}
#endif



//-------------------------------------------
// Window
//-------------------------------------------
void LT_CloseWindow(void) { shouldClose = true; }

void LinuxSwapBuffer(void) {

}

void X11ProcEvent(Window xWindow, XEvent* event, int32 screen) {
  static const char *msg = "Hello, World!";

  if (event->type == Expose) {
    XFillRectangle(display, xWindow, DefaultGC(display, screen), 20, 20, 10, 10);
    XDrawString(display, xWindow, DefaultGC(display, screen), 10, 50, msg, std::strlen(msg));
  }
  if (event->type == KeyPress) {
    if (event->xkey.keycode == 9)
      LT_CloseWindow();
  }
}

//-------------------------------------------
// Graphics
//-------------------------------------------

LT::LoadProc InitOpenGL(void) {
  return nullptr;
}

LT::SwapBuffersFunc GetPlatformSwapBuffer(void) {
  return LinuxSwapBuffer;
}

//-------------------------------------------
// Input
//-------------------------------------------

void PlatformInitInput(int32* in_keyStates) {
  int32 scanCodeMin, scanCodeMax, width;
  XDisplayKeycodes(display, &scanCodeMin, &scanCodeMax);
  KeySym* keysyms = XGetKeyboardMapping(display,
                                        scanCodeMin,
                                        scanCodeMax - scanCodeMin + 1,
                                        &width);
  for (int32 scancode = scanCodeMin;  scancode <= scanCodeMax;  scancode++)
  {
    // Translate the un-translated key codes using traditional X11 KeySym
    // lookups
    const size_t base = (scancode - scanCodeMin) * width;
    X11TranslateKeys(&keysyms[base]);
  }
}

uint8 PlatformGetKeyState(int32 keyState) {
  return 0;
}

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
}

//-------------------------------------------
// Threads
//-------------------------------------------
namespace LT {
LT::Thread* Platform::ThreadCreate(LT::Thread *thread, LT::ThreadFuncWrapper funcWrapper) {
  pthread_t threadID;
  void* threadArg = nullptr;
  int32 result = pthread_create(
          &threadID,
          nullptr,
          funcWrapper,
          threadArg);


  if (result != 0) {
    log_error("Failed to create thread.");
    LinuxHandleError(ERROR_PLATFORM_THREAD_CREATE);
  }

  LT::ThreadLinux nixThd = LT::ThreadLinux(threadID);
  std::memcpy(thread->reserved, &nixThd, sizeof(LT::ThreadLinux));
  return thread;
}

void Platform::ThreadStart(const LT::Thread *thread) {
  //Does nothing
}

void Platform::GetCurrent(const LT::Thread *thread) {
  pthread_t threadID = pthread_self();
  LT::ThreadLinux nixThd = LT::ThreadLinux(threadID);
  std::memcpy(const_cast<void*>(reinterpret_cast<const void*>(thread->reserved)), &nixThd, sizeof(LT::ThreadLinux));
}

void Platform::ThreadJoin(const LT::Thread* thread) {
  pthread_join(reinterpret_cast<uint64>(thread->reserved), const_cast<void**>(reinterpret_cast<void* const*>(&thread->exitCode)));
}

void Platform::ThreadSleep(const LT::Thread* thread, const uint64 miliseconds) {
  struct timespec ts;
  int res;

  if (miliseconds < 0)
    return;

  ts.tv_sec = miliseconds / 1000;
  ts.tv_nsec = (miliseconds % 1000) * 1000000;

  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);
}

LT_NORETURN void Platform::ThreadExit(const int32 exit_code) {
  void *exitCode = reinterpret_cast<void*>(exit_code);
  pthread_exit(exitCode);
}

void Platform::ThreadGetExitCode(LT::Thread* thread) {
  if (!pthread_tryjoin_np(reinterpret_cast<uint64>(thread->reserved), reinterpret_cast<void**>(&thread->exitCode) )) {
    log_error("Thread still active");
  } else {
    thread->isValid = false;
  }
}

void Platform::ThreadDestroy(LT::Thread* thread) {
  thread->isValid = false;
}
}

//-------------------------------------------
// Threading Locks
//-------------------------------------------
namespace LT {
LT::ThreadLock* PlatformThreadLockCreate() {
  pthread_mutex_t* lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(lock, nullptr);
  return reinterpret_cast<LT::ThreadLock*>(lock);
}

void PlatformThreadLockLock(LT::ThreadLock* lock) {
  pthread_mutex_lock(reinterpret_cast<pthread_mutex_t*>(lock));
}

void PlatformThreadLockUnock(LT::ThreadLock* lock) {
  pthread_mutex_unlock(reinterpret_cast<pthread_mutex_t*>(lock));
}

void PlatformThreadLockDestroy(LT::ThreadLock* lock) {
  pthread_mutex_destroy(reinterpret_cast<pthread_mutex_t*>(lock));
}
}

static LT_NORETURN void LinuxHandleError(int32 in_exitCode) {
  exit(in_exitCode);
}
