#include "Engine.hpp"

#include <GameManager.hpp>
#include <Networking.hpp>
#include <log.hpp>
#include <memory>

#include "GraphicsAPI.hpp"
#include "Input.hpp"

namespace LT {
void Engine_Start(const ConfigArgs* args) {
  log_trace("Engine is starting...");

  // load config file
  const char* ip = "127.0.0.1";
  Config config(ip, sizeof(ip));
  config.graphic_api = GRAPHIC_API::OPENGL;
  config.networking_support = true;
  config.is_server = false;
  config.port = 27854;

  // override config with args
  if (args != nullptr) {
    config.is_server = args->isServer;

    // free mem after use
    delete args;
  }

  GraphicsAPI_Init(GRAPHIC_API::OPENGL);

  // it needs a window so it should be after
  // graphics init (which it creates the window)
  InputInit();

  // GameStateInit("game");

  api.clearScreenColor(LT::ColorRGBA(1.0f, 0.1f, 0.5f));

  log_info("Engine started!");
}

void Engine_Run(const double deltaTime) {
  // GameStateUpdateCurrent(deltaTime);
  /*
  if (LT::GetKeyState(LT::INPUT_KEY::KEYS_COUNT) ==
      LT::INPUT_STATE::LT_KEY_PRESSED)
    LT::CloseWindow();
    */
  api.clearScreen(LT::BUFFER_BIT::LT_COLOR_BIT);

  api.swapBuffers();
}

void Engine_Shutdown() {
  log_trace("Engine is shuting down...");
  log_info("Engine shut down.");
}
}  // namespace LT
