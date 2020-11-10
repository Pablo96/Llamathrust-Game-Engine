/**
 * @file OpenGL.c
 * @author Pablo Narvaja
 * @brief Here is all the code for opengl implementation.
 **/

#include "Common.hpp"
#include "GraphicsAPI.hpp"
#include <glad/glad.hpp>
#include <log.hpp>

namespace LT {
    static uint8 isDepthTestingEnabled = false;
    static uint8 isStencilTestingEnabled = false;
    static uint8 isScissorTestingEnabled = false;
    static uint8 isAlphaBlendingEnabled = false;

    void GL_ClearScreenColor8(const ColorRGBA8 in_value) {
        log_info("GL: Clear color 8bits per channel.");
        ColorRGBA color = { in_value.r / 255.0f, in_value.g / 255.0f,
                           in_value.b / 255.0f, in_value.a / 255.0f };
        GL_ClearScreenColor(&color);
    }

    void GL_ClearScreenColor(const ColorRGBA* in_value) {
        log_info("GL: Clear color.");
        glClearColor(in_value->r, in_value->g, in_value->b, in_value->a);
    }

    void GL_ClearScreen(const BUFFER_BIT in_value) {
        GLenum bufferBit = 0;
        if (in_value & BUFFER_BIT::LT_COLOR_BIT) {
            bufferBit |= GL_COLOR_BUFFER_BIT;
        }
        if (in_value & BUFFER_BIT::LT_DEPTH_BIT) {
            bufferBit |= GL_DEPTH_BUFFER_BIT;
        }
        if (in_value & BUFFER_BIT::LT_STENCIL_BIT) {
            bufferBit |= GL_STENCIL_BUFFER_BIT;
        }
        glClear(bufferBit);
    }

    void GL_EnableDepthTesting(const uint8 in_value) {
        if (in_value == false && isDepthTestingEnabled == true) {
            glDisable(GL_DEPTH);
            isDepthTestingEnabled = false;
        }
        if (in_value == true && isDepthTestingEnabled == false) {
            glEnable(GL_DEPTH);
            isDepthTestingEnabled = true;
        }
    }

    void GL_EnableStencilTesting(const uint8 in_value) {
        if (in_value == false && isStencilTestingEnabled == true) {
            glDisable(GL_STENCIL_TEST);
            isStencilTestingEnabled = false;
        }
        if (in_value == true && isStencilTestingEnabled == false) {
            glEnable(GL_STENCIL_TEST);
            isStencilTestingEnabled = true;
        }
    }

    void GL_EnableAlphaBlending(const uint8 in_value) {
        if (in_value == false && isAlphaBlendingEnabled == true) {
            glDisable(GL_BLEND);
            isAlphaBlendingEnabled = false;
        }
        if (in_value == true && isAlphaBlendingEnabled == false) {
            glEnable(GL_BLEND);
            isAlphaBlendingEnabled = true;
        }
    }

    void GL_EnableScissorTesting(const uint8 in_value) {
        if (in_value == false && isScissorTestingEnabled == true) {
            glDisable(GL_SCISSOR_TEST);
            isScissorTestingEnabled = false;
        }
        if (in_value == true && isScissorTestingEnabled == false) {
            glEnable(GL_SCISSOR_TEST);
            isScissorTestingEnabled = true;
        }
    }

    void GL_SetScissor(const Rect* in_value) {
        glScissor(in_value->x, in_value->y, in_value->width, in_value->height);
    }

    void GL_SetViewport(const Rect* in_value) {
        glViewport(in_value->x, in_value->y, in_value->width, in_value->height);
    }

}