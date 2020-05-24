#include "GraphicsAPI.h"
#include <Platform.h>
#include <glad/glad.h>
#include <log.h>
#include <stdlib.h>

GraphicsAPI api = {0};

// Funcitons declarations
static void GL_EnableDepthTesting(const uint8 in_value);
static void GL_EnableAlphaBlending(const uint8 in_value);
static void GL_EnableScissorTesting(const uint8 in_value);
static void GL_SetViewport(const Rect *in_value);

static void VK_EnableDepthTesting(const uint8 in_value) {}
static void VK_EnableAlphaBlending(const uint8 in_value) {}
static void VK_EnableScissorTesting(const uint8 in_value) {}
static void VK_SetViewport(const Rect *in_value) {}

void LT_GraphicsAPI_Init(const API in_api) {
  switch (in_api) {
  case OPENGL: {
    LoadProc proc = InitOpenGL();
    if (gladLoadGLLoader(proc) == 0) {
      log_fatal("Could not initialize glad.");
      exit(50);
    }
    api.enableAlphaBlending = GL_EnableAlphaBlending;
    api.enableDepthTesting = GL_EnableDepthTesting;
    api.enableScissorTesting = GL_EnableScissorTesting;
    api.setViewport = GL_SetViewport;
    break;
  }
  case VULKAN: {
    log_fatal("API %d not implemented!", in_api);
    exit(54);
    api.enableAlphaBlending = VK_EnableAlphaBlending;
    api.enableDepthTesting = VK_EnableDepthTesting;
    api.enableScissorTesting = VK_EnableScissorTesting;
    api.setViewport = VK_SetViewport;
  }
  }
  api.swapBuffers = GetPlatformSwapBuffer();

  log_info("GraphicsAPI initialized.");
}

void GL_EnableDepthTesting(const uint8 in_value) {}
void GL_EnableAlphaBlending(const uint8 in_value) {}
void GL_EnableScissorTesting(const uint8 in_value) {}
void GL_SetViewport(const Rect *in_value) {}
