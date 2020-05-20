#include "Engine.h"
#include "GraphicsAPI.h"
#include <Platform.h>
#include <log.h>

void Engine_Start() {
    log_trace("Engine is starting...");

    LT_GraphicsAPI_Init(OPENGL);
    
    log_info("Engine started!");
}

void Engine_Run(const decimal deltaTime) {

    // swap buffers in all windows
    api.swapBuffers(window);
}

void Engine_Shutdown() {
    log_trace("Engine is shuting down...");
    log_info("Engine is off.");
}
