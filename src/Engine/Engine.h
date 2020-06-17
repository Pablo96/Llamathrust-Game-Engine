#pragma once
#include <Common.h>

extern void LT_CloseWindow(void);

void Engine_Start(void);

void Engine_Run(const double deltaTime);

void Engine_Shutdown(void);
