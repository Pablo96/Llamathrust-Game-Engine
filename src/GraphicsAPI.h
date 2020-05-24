#pragma once

typedef enum _APIs { OPENGL, VULKAN } API;

typedef struct _Rect {
  int x;
  int y;
  int width;
  int height;
} Rect;

typedef struct _GraphicsAPI {
  void (*enableAlphaBlending)(const uint8 in_value);
  void (*enableDepthTesting)(const uint8 in_value);
  void (*enableScissorTesting)(const uint8 in_value);
  void (*setViewport)(const Rect *in_value);
  void (*swapBuffers)(void);
} GraphicsAPI;

extern GraphicsAPI api;

void LT_GraphicsAPI_Init(const API in_api);
