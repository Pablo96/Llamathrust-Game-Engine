#include "Engine.h"
#include "GraphicsAPI.h"
#include "Input.h"
#include "threading/Thread.h"
#include <GameManager.h>
#include <log.h>

static void function(void* param) {
  log_info("Initialized thread");

  Thread* this = LT_Thread_GetCurrent();

  LT_Thread_Sleep(this, 4000);

  log_info("Finished thread");
}

void Engine_Start() {
  Thread* thread = LT_Thread_Create(function, NULL);
  
  log_trace("Engine is starting...");

  LT_InputInit();

  LT_GraphicsAPI_Init(LT_OPENGL);

  LT_Thread_Join(thread);

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
