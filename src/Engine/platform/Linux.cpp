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

// Windowing
LT_Window window;
static bool shouldClose = false;

// Linux
static Display *display;

static void inputMethodDestroyCallback(XIM im, XPointer clientData, XPointer callData) {}
static void inputContextDestroyCallback(XIC ic, XPointer clientData, XPointer callData) {}

#ifndef LT_NO_MAIN //used for running tests
int main(int32 argc, const char **argv) {
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }


  int screen = DefaultScreen(display);
  int depth = DefaultDepth(display, screen);
  Visual* visual = DefaultVisual(display, screen);
  Window root = RootWindow(display, screen);
  XWindowAttributes windowAttributes = { 0 };
  
  // Create a colormap based on the visual used by the current context
  windowAttributes.colormap = XCreateColormap(display, root, visual, AllocNone);
  windowAttributes.your_event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                                PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
                                ExposureMask | FocusChangeMask | VisibilityChangeMask |
                                EnterWindowMask | LeaveWindowMask | PropertyChangeMask;
  
  Window window = XCreateWindow(
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
  XMapWindow(display, window);

  // Input setup
  XIM im = XOpenIM(display, 0, NULL, NULL);
  if (!im)
    return;
  
  XIMCallback callback;
  callback.callback = (XIMProc) inputMethodDestroyCallback;
  callback.client_data = NULL;
  XSetIMValues(im, XNDestroyCallback, &callback, NULL);

  callback.callback = (XIMProc) inputContextDestroyCallback;
  callback.client_data = (XPointer) window;
  XIC ic = XCreateIC(im,
                    XNInputStyle,
                    XIMPreeditNothing | XIMStatusNothing,
                    XNClientWindow,
                    window,
                    XNFocusWindow,
                    window,
                    XNDestroyCallback,
                    &callback,
                    NULL);

  if (!im)
    return;
  
  unsigned long filter = 0;
  if (XGetICValues(ic, XNFilterEvents, &filter, NULL) == NULL)
  {
      XSelectInput(display,
                  window,
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
      X11ProcEvent(window, &e, screen);
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

void X11ProcEvent(Window window, XEvent* event, int32 screen) {
  static const char *msg = "Hello, World!";

  if (event->type == Expose) {
    XFillRectangle(display, window, DefaultGC(display, screen), 20, 20, 10, 10);
    XDrawString(display, window, DefaultGC(display, screen), 10, 50, msg, strlen(msg));
  }
  if (event->type == KeyPress) {
    if (event->xkey.keycode == 9)
      LT_CloseWindow();
  }
}

//-------------------------------------------
// Graphics
//-------------------------------------------

LoadProc InitOpenGL(void) {
  return NULL;
}
SwapBuffersFunc GetPlatformSwapBuffer(void) {
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

LT_INPUT_KEY X11TranslateKeys(int32 key) {
  log_info("KeyCode %p", key);
  return 0;
}
//-------------------------------------------
// Shared libs
//-------------------------------------------

void* PlatformLoadSharedLib(const char* name) {
  return dlopen(name, RTLD_LAZY);
}

void* PlatformGetProc(const void* in_lib, const char* in_name) {
  return dlsym(in_lib, in_name);
}

//-------------------------------------------
// Threads
//-------------------------------------------

Thread *PlatformThreadCreate(const Thread *thread, ThreadFuncWrapper funcWrapper) {
  pthread_t *threadID;
  int32 result = pthread_create(
          threadID,
          NULL,
          funcWrapper,
          &threadID);


  if (result != 0) {
    log_error("Failed to create thread.");
    LinuxHandleError(ERROR_PLATFORM_THREAD_CREATE);
  }

  ThreadLinux nixThread = { .id = threadID };
  memcpy(thread->reserved, &nixThread, sizeof(ThreadLinux));
  return thread;
}

void PlatformThreadStart(const Thread *thread) {
  //Does nothing
}

void PlatformGetCurrent(const Thread *thread) {
  pthread_t threadID = pthread_self();
  ThreadLinux nixThd = {.id = threadID };
  memcpy(thread->reserved, &nixThd, sizeof(ThreadLinux));
}

void PlatformThreadJoin(const Thread* thread) {
  pthread_join(thread->reserved, &thread->exitCode);
}

void PlatformThreadSleep(const Thread* thread, const uint64 miliseconds) {
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

void PlatformThreadExit(const int16 exit_code) {
  void *exitCode = (void*) exit_code;
  pthread_exit(exitCode);
}

void PlatformThreadGetExitCode(Thread* thread) {
  if (!pthread_tryjoin_np(thread->reserved, &thread->exitCode)) {
    log_error("Thread still active");
  } else {
    thread->isValid = false;
  }
}

void PlatformThreadDestroy(Thread* thread) {
  thread->isValid = false;
}

//-------------------------------------------
// Threading Locks
//-------------------------------------------

ThreadLock* PlatformThreadLockCreate() {
  pthread_mutex_t* lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(lock, NULL);
  return (ThreadLock*)lock;
}

void PlatformThreadLockLock(ThreadLock* lock) {
  pthread_mutex_lock(lock);
}

void PlatformThreadLockUnock(ThreadLock* lock) {
  pthread_mutex_unlock(lock);
}

void PlatformThreadLockDestroy(ThreadLock* lock) {
  pthread_mutex_destroy(lock);
}

LT_NORETURN void LinuxHandleError(int32 in_exitCode) {
  exit(in_exitCode);
}
