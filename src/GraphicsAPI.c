#include "GraphicsAPI.h"
#include <log.h>

static GraphicsAPI api = {0};

void GraphicsAPI_Init(void) {
    log_info("Graphics API initialized!");
}
