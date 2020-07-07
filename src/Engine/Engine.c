#include "Engine.h"
#include "GraphicsAPI.h"
#include "Input.h"
#include <Networking.h>
#include <GameManager.h>
#include <log.h>

typedef void (RunFunc*)(const double deltaTime);

static RunFunc run;

void ServerRunFunc(const double deltaTime);
void ClientRunFunc(const double deltaTime);
void LocalRunFunc(const double deltaTime);

void Engine_Start(const ConfigArgs* args) {
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

  // Set main run function
  if (!config.networking_support)
    run = LocalRunFunc;
  else if (config.is_server)
    run = ServerRunFunc;
  else
    run = ClientRunFunc;
  

  LT_InputInit();
  
  LT_GraphicsAPI_Init(config.graphic_api);

  LT_NetworkingInit(&config);

  LT_GameStateInit("game");

  log_info("Engine started!");
}

void Engine_Run(const double deltaTime) {
  run(deltaTime);
}

void Engine_Shutdown() {
  log_trace("Engine is shuting down...");
  log_info("Engine shut down.");
}


// Main Funcitons


void ServerRunFunc(const double deltaTime) {
  uint32 read;
  byte readable[128];

  LT_NetworkingListen();

  for (uint32 i=0; i < clientsCount; i++) {
    LT_NetworkingRead(readable, &read, 128);
    readable[read] = 0;
    log_info("client %u: %s", i, readable);
  }
}

void ClientRunFunc(const double deltaTime) {

}

void LocalRunFunc(const double deltaTime) {
  LT_GameStateUpdateCurrent(deltaTime);
  api.swapBuffers();
}
