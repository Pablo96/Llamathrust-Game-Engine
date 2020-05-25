#include "Engine.h"
#include "GraphicsAPI.h"
#include "Input.h"
#include <Platform.h>
#include <log.h>

void Engine_Start() {
  log_trace("Engine is starting...");

  LT_InputInit();
  LT_GraphicsAPI_Init(LT_OPENGL);

  ColorRGBA color = {0.1f, 0.1f, 0.5f, 1.0f};
  api.clearScreenColor(&color);
  api.enableDepthTesting(LT_TRUE);
  
  log_info("Engine started!");
}

void Engine_Run(const decimal deltaTime) {
  api.clearScreen(LT_COLOR_BIT);

  LT_INPUT_STATE escState = LT_GetKeyState(LT_KEY_ESCAPE);
  LT_INPUT_STATE spaState = LT_GetKeyState(LT_KEY_SPACEBAR);
  if (escState == LT_KEY_PRESSED || spaState == LT_KEY_PRESSED) {
    LT_CloseWindow();
  }

  api.swapBuffers();
}

void Engine_Shutdown() {
  log_trace("Engine is shuting down...");
  log_info("Engine is off.");
}
