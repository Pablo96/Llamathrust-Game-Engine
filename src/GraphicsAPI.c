#include "GraphicsAPI.h"
#include "platform/Win32.h"
#include <log.h>
#include <stdlib.h>
#include <glad/glad.h>

GraphicsAPI api = {0};

void LT_GraphicsAPI_Init(const API in_api) {
    switch (in_api) {
        case OPENGL:
        {
            Win32loadproc proc = Win32InitOpenGL();
            if (gladLoadGLLoader(proc) == 0) {
                log_fatal("Could not initialize glad.");
                exit(50);
            }
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
