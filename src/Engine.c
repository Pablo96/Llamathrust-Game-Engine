#include "Engine.h"
#include <stdio.h>
#include <log.h>

void Engine_Start() {
    log_trace("Engine is starting...\n");
    log_info("Engine started!\n");
}

void Engine_Run(const decimal deltaTime) {

}

void Engine_Shutdown() {
    log_trace("Engine is shuting down...\n");
    log_info("Engine is off.\n");
}
