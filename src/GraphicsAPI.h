#pragma once

typedef struct _GraphicsAPI {
    void (*swapBuffers)(void);
} GraphicsAPI;

void GraphicsAPI_Init(void);
