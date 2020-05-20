#include "GraphicsAPI.h"
#include <log.h>
#include "platform/Win32.h"

static GraphicsAPI api = {0};

void GraphicsAPI_Init(void) {
    log_info("Graphics API initialized!");
    api.swapBuffers = Win32SwapBuffer;
}
