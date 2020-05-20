#pragma once
#include <Platform.h>

typedef enum _APIs {
    OPENGL,
    VULKAN
} API;

typedef struct _GraphicsAPI {
    void (*swapBuffers)(const Window*);
} GraphicsAPI;

extern GraphicsAPI api;

void LT_GraphicsAPI_Init(const API in_api);
