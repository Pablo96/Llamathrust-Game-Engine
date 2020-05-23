#pragma once

typedef enum _APIs { OPENGL, VULKAN } API;

typedef struct _GraphicsAPI {

  void (*swapBuffers)(void);
} GraphicsAPI;

extern GraphicsAPI api;

void LT_GraphicsAPI_Init(const API in_api);
