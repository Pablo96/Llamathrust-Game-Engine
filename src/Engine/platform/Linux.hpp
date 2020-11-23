#pragma once
#include "Common.hpp"
#include "Platform.hpp"
#include <stdnoreturn.h>
#include "../Input.hpp"

void LinuxSwapBuffer(void);

LT::INPUT_KEY X11TranslateKeys(int32 key);

typedef union _XEvent XEvent;
typedef unsigned long XID;
typedef XID Window;
void X11ProcEvent(Window window, XEvent* event, int32 screen);
