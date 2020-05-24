#include "GraphicsAPI.h"
#include <Platform.h>
#include <glad/glad.h>
#include <log.h>
#include <stdlib.h>

GraphicsAPI api = {0};

// OPENGL declarations
static void GL_ClearScreenColor8(const ColorRGBA8 in_value);
static void GL_ClearScreenColor(const ColorRGBA *in_value);
static void GL_ClearScreen(const uint8 in_value);
static void GL_EnableDepthTesting(const uint8 in_value);
static void GL_EnableAlphaBlending(const uint8 in_value);
static void GL_EnableScissorTesting(const uint8 in_value);
static void GL_SetViewport(const Rect *in_value);

// VULKAN declarations
static void VK_SetViewport(const Rect *in_value) {}

void LT_GraphicsAPI_Init(const API in_api) {
  switch (in_api) {
  case LT_OPENGL: {
    LoadProc proc = InitOpenGL();
    if (gladLoadGLLoader(proc) == 0) {
      log_fatal("Could not initialize glad.");
      exit(50);
    }
    api.clearScreenColor8 = GL_ClearScreenColor8;
    api.clearScreenColor = GL_ClearScreenColor;
    api.clearScreen = GL_ClearScreen;
    api.enableAlphaBlending = GL_EnableAlphaBlending;
    api.enableDepthTesting = GL_EnableDepthTesting;
    api.enableScissorTesting = GL_EnableScissorTesting;
    api.setViewport = GL_SetViewport;
    break;
  }
  case LT_VULKAN: {
    log_fatal("API %d not implemented!", in_api);
    exit(54);
    api.setViewport = VK_SetViewport;
  }
  }
  api.swapBuffers = GetPlatformSwapBuffer();

  log_info("GraphicsAPI initialized.");
}

void GL_ClearScreenColor8(const ColorRGBA8 in_value) {}
void GL_ClearScreenColor(const ColorRGBA *in_value) {}
void GL_ClearScreen(const uint8 in_value) {}
void GL_EnableDepthTesting(const uint8 in_value) {}
void GL_EnableAlphaBlending(const uint8 in_value) {}
void GL_EnableScissorTesting(const uint8 in_value) {}
void GL_SetViewport(const Rect *in_value) {}
