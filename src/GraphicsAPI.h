#pragma once
#include <Platform.h>

typedef struct _GraphicsAPI {
    void (*swapBuffers)(const Window*);
} GraphicsAPI;

extern GraphicsAPI api;

void GraphicsAPI_Init(void);
