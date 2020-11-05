/**
 * @file OpenGL.c
 * @author Pablo Narvaja
 * @brief Here is all the code for opengl implementation.
 **/

#include "Common.h"
#include "GraphicsAPI.h"
#include <glad/glad.h>
#include <log.h>

static uint8 isDepthTestingEnabled = LT_FALSE;
static uint8 isStencilTestingEnabled = LT_FALSE;
static uint8 isScissorTestingEnabled = LT_FALSE;
static uint8 isAlphaBlendingEnabled = LT_FALSE;

void GL_ClearScreenColor8(const ColorRGBA8 in_value) {
  log_info("GL: Clear color 8bits per channel.");
  ColorRGBA color = {in_value.r / 255.0f, in_value.g / 255.0f,
                     in_value.b / 255.0f, in_value.a / 255.0f};
  GL_ClearScreenColor(&color);
}

void GL_ClearScreenColor(const ColorRGBA *in_value) {
  log_info("GL: Clear color.");
  glClearColor(in_value->r, in_value->g, in_value->b, in_value->a);
}

void GL_ClearScreen(const BufferBit in_value) {
  GLenum bufferBit = 0;
  if (in_value & LT_COLOR_BIT) {
    bufferBit |= GL_COLOR_BUFFER_BIT;
  }
  if (in_value & LT_DEPTH_BIT) {
    bufferBit |= GL_DEPTH_BUFFER_BIT;
  }
  if (in_value & LT_STENCIL_BIT) {
    bufferBit |= GL_STENCIL_BUFFER_BIT;
  }
  glClear(bufferBit);
}

void GL_EnableDepthTesting(const uint8 in_value) {
  if (in_value == LT_FALSE && isDepthTestingEnabled == LT_TRUE) {
    glDisable(GL_DEPTH);
    isDepthTestingEnabled = LT_FALSE;
  }
  if (in_value == LT_TRUE && isDepthTestingEnabled == LT_FALSE) {
    glEnable(GL_DEPTH);
    isDepthTestingEnabled = LT_TRUE;
  }
}

void GL_EnableStencilTesting(const uint8 in_value) {
  if (in_value == LT_FALSE && isStencilTestingEnabled == LT_TRUE) {
    glDisable(GL_STENCIL_TEST);
    isStencilTestingEnabled = LT_FALSE;
  }
  if (in_value == LT_TRUE && isStencilTestingEnabled == LT_FALSE) {
    glEnable(GL_STENCIL_TEST);
    isStencilTestingEnabled = LT_TRUE;
  }
}

void GL_EnableAlphaBlending(const uint8 in_value) {
  if (in_value == LT_FALSE && isAlphaBlendingEnabled == LT_TRUE) {
    glDisable(GL_BLEND);
    isAlphaBlendingEnabled = LT_FALSE;
  }
  if (in_value == LT_TRUE && isAlphaBlendingEnabled == LT_FALSE) {
    glEnable(GL_BLEND);
    isAlphaBlendingEnabled = LT_TRUE;
  }
}

void GL_EnableScissorTesting(const uint8 in_value) {
  if (in_value == LT_FALSE && isScissorTestingEnabled == LT_TRUE) {
    glDisable(GL_SCISSOR_TEST);
    isScissorTestingEnabled = LT_FALSE;
  }
  if (in_value == LT_TRUE && isScissorTestingEnabled == LT_FALSE) {
    glEnable(GL_SCISSOR_TEST);
    isScissorTestingEnabled = LT_TRUE;
  }
}

void GL_SetScissor(const Rect *in_value) {
  glScissor(in_value->x, in_value->y, in_value->width, in_value->height);
}

void GL_SetViewport(const Rect *in_value) {
  glViewport(in_value->x, in_value->y, in_value->width, in_value->height);
}