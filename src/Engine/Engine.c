#include "Engine.h"
#include "GraphicsAPI.h"
#include "Input.h"
#include <GameManager.h>
#include <log.h>

void Engine_Start() {
  log_trace("Engine is starting...");

  LT_InputInit();
  
  LT_GraphicsAPI_Init(LT_OPENGL);

  LT_GameStateInit("game");

  log_info("Engine started!");
}

void Engine_Run(const double deltaTime) {
  LT_GameStateUpdateCurrent(deltaTime);
  api.swapBuffers();
}

void Engine_Shutdown() {
  log_trace("Engine is shuting down...");
  log_info("Engine shut down.");
}
