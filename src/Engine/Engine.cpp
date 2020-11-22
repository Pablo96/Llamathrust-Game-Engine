#include "Engine.hpp"
#include "GraphicsAPI.hpp"
#include "Input.hpp"
#include <GameManager.hpp>
#include <Networking.hpp>
#include <log.hpp>
#include <memory>

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

        InputInit();

        GraphicsAPI_Init(config.graphic_api);

        GameStateInit("game");

        log_info("Engine started!");
    }

    void Engine_Run(const double deltaTime) {
        GameStateUpdateCurrent(deltaTime);
        api.swapBuffers();
    }

    void Engine_Shutdown() {
        log_trace("Engine is shuting down...");
        log_info("Engine shut down.");
    }
}
