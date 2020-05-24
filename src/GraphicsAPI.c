#include "GraphicsAPI.h"
#include <Platform.h>
#include <glad/glad.h>
#include <log.h>
#include <stdlib.h>

GraphicsAPI api = {0};

void LT_GraphicsAPI_Init(const API in_api) {
  switch (in_api) {
  case OPENGL: {
    LoadProc proc = InitOpenGL();
    if (gladLoadGLLoader(proc) == 0) {
      log_fatal("Could not initialize glad.");
      exit(50);
    }
    break;
  }
  case VULKAN: {
    log_fatal("API %d not implemented!", in_api);
    exit(54);
  }
  }
  api.swapBuffers = GetSwapBuffer();

  log_info("GraphicsAPI initialized.");
}
