#include "GraphicsAPI.hpp"

#include <stdlib.h>

#include <ErrorCodes.hpp>
#include <Platform.hpp>
#include <glad/glad.hpp>
#include <log.hpp>
#include <stdexcept>

#include "OpenGL.hpp"

namespace LT {

GraphicsAPI api = {};

// VULKAN declarations
static void VK_Init();
static void VK_SetViewport(const Rect* in_value) {}

void GraphicsAPI_Init(const GRAPHIC_API in_api) {
  log_trace("Intializing GraphicsAPI...");

  switch (in_api) {
    case GRAPHIC_API::OPENGL: {
      LoadProc proc = Platform::InitOpenGL();
      if (gladLoadGLLoader(proc) == 0) {
        std::string msg = GET_ERROR_MSG(ERROR_GRAPHICS_API_INITIALIZATION);
        log_fatal("%s: %s", msg.c_str(), "Could not initialize glad.");
        throw new std::runtime_error(msg);
      }

      api.clearScreenColor8 = GL_ClearScreenColor8;
      api.clearScreenColor = GL_ClearScreenColor;
      api.clearScreen = GL_ClearScreen;
      api.enableAlphaBlending = GL_EnableAlphaBlending;
      api.enableDepthTesting = GL_EnableDepthTesting;
      api.enableStencilTesting = GL_EnableStencilTesting;
      api.enableScissorTesting = GL_EnableScissorTesting;
      api.setScissor = GL_SetScissor;
      api.setViewport = GL_SetViewport;
      break;
    }
    case GRAPHIC_API::VULKAN: {
      std::string msg = GET_ERROR_MSG(ERROR_GRAPHICS_API_NOT_IMPLEMENTED);
      log_fatal("%s: VULKAN", msg.c_str());
      throw new std::runtime_error(msg);
    }
  }
  api.swapBuffers = Platform::GetPlatformSwapBuffer();

  log_info("GraphicsAPI initialized.");
}

void VK_Init() {}
}  // namespace LT