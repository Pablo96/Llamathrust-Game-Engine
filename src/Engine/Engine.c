#include "Engine.h"
#include "GraphicsAPI.h"
#include "Input.h"
#include <GameManager.h>
#include <Networking.h>
#include <log.h>

void Engine_Start(const ConfigArgs *args) {
  log_trace("Engine is starting...");

  // load config file
  Config config = {
      .graphic_api = LT_OPENGL,
      .networking_support = LT_TRUE,
      .is_server = LT_FALSE,
      .port = 27854,
      .ip = "127.0.0.1",
  };

  // override config with args
  if (args != NULL) {
    config.is_server = args->isServer;

    // free mem after use
    free(args);
  }

  LT_InputInit();

  LT_GraphicsAPI_Init(config.graphic_api);

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
