#include "Engine.h"
#include "GraphicsAPI.h"
#include "Input.h"
#include <Platform.h>
#include <glad/glad.h>
#include <log.h>
void Engine_Start() {
  log_trace("Engine is starting...");

  LT_InputInit();
  LT_GraphicsAPI_Init(OPENGL);

  glClearColor(0.1f, 0.1f, 0.5f, 1.0f);

  log_info("Engine started!");
}

void Engine_Run(const decimal deltaTime) {
  glClear(GL_COLOR_BUFFER_BIT);

  LT_INPUT_STATE escState = LT_GetKeyState(ESCAPE);
  LT_INPUT_STATE spaState = LT_GetKeyState(SPACEBAR);
  log_info("Escape state: %u\tSpace state: %u", escState, spaState);
  
  if (escState == LT_KEY_PRESSED || spaState == LT_KEY_PRESSED) {
    LT_CloseWindow();
  }

  api.swapBuffers();
}

void Engine_Shutdown() {
  log_trace("Engine is shuting down...");
  log_info("Engine is off.");
}
