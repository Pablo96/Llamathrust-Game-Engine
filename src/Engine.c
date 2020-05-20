#include "Engine.h"
#include "GraphicsAPI.h"
#include <Platform.h>
#include <log.h>

void Engine_Start() {
    log_trace("Engine is starting...");

    LT_GraphicsAPI_Init(OPENGL);
    LT_CreateWindow(720, 480, "Game x64 (llamathrust) [clang]");
    
    log_info("Engine started!");
}

void Engine_Run(const decimal deltaTime) {

    // swap buffers in all windows
    for (uint32 i = 0; i < windowsCount; i++) {
        api.swapBuffers(windowsVec + i);
    }
}

void Engine_Shutdown() {
    log_trace("Engine is shuting down...");
    log_info("Engine is off.");
}
