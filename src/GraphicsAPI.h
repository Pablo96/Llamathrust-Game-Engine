#pragma once
#include <Common.h>

typedef enum _APIs { LT_OPENGL, LT_VULKAN } API;

typedef struct _Rect {
  int x;
  int y;
  int width;
  int height;
} Rect;

typedef struct _Color8 {
  union {
    struct {
      uint8 r;
      uint8 g;
      uint8 b;
      uint8 a;
    };
    uint32 hash;
  };
} ColorRGBA8;

typedef struct _Color {
  float r;
  float g;
  float b;
  float a;
} ColorRGBA;

typedef enum _BufferBit { LT_COLOR_BIT, LT_DEPTH_BIT, LT_STENCIL_BIT } BufferBit;

typedef struct _GraphicsAPI {
  void (*clearScreenColor8)(const ColorRGBA8 in_value);
  void (*clearScreenColor)(const ColorRGBA *in_value);
  void (*clearScreen)(const BufferBit in_value);
  void (*enableAlphaBlending)(const uint8 in_value);
  void (*enableDepthTesting)(const uint8 in_value);
  void (*enableScissorTesting)(const uint8 in_value);
  void (*setViewport)(const Rect *in_value);
  void (*swapBuffers)(void);
} GraphicsAPI;

extern GraphicsAPI api;

void LT_GraphicsAPI_Init(const API in_api);
