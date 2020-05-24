#include "Engine.h"
#include "GraphicsAPI.h"
#include <Platform.h>
#include <glad/glad.h>
#include <log.h>

void Engine_Start() {
  log_trace("Engine is starting...");

  LT_GraphicsAPI_Init(OPENGL);

  glClearColor(0.1f, 0.1f, 0.5f, 1.0f);

  log_info("Engine started!");
}

void Engine_Run(const decimal deltaTime) {
  glClear(GL_COLOR_BUFFER_BIT);

  // swap buffers in all windows
  api.swapBuffers();
}

void Engine_Shutdown() {
  log_trace("Engine is shuting down...");
  log_info("Engine is off.");
}
