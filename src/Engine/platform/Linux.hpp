#pragma once
#include "Common.hpp"
#include "Platform.hpp"
#include <stdnoreturn.h>
#include "../Input.hpp"

void LinuxSwapBuffer(void);

typedef union _XEvent XEvent;

void X11ProcEvent(XEvent* event);
