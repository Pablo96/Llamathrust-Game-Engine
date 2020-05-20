#include "GraphicsAPI.h"
#include <log.h>
#include "platform/Win32.h"
#include <stdlib.h>

GraphicsAPI api = {0};

void LT_GraphicsAPI_Init(const API in_api) {
    switch (in_api) {
        case OPENGL:
        {
            Win32InitOpenGL();
            break;
        }
        case VULKAN:
        {
            log_fatal("API %d not implemented!", in_api);
            exit(54);
        }
    }
    api.swapBuffers = Win32SwapBuffer;

    log_info("GraphicsAPI initialized.");
}
