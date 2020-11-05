/**
 * @file GraphicsAPI.h
 * @author Pablo Narvaja
 * @brief Here is all the code for graphics api abstraction.
 **/
#pragma once
#include <Common.hpp>

namespace LT {
    /**
     * @enum GRAPHIC_API
     * @brief Available GraphicsAPIs.
     * @tag LT_OPENGL:
     *    @brief OpenGL 4+
     *    @note available for desktops but Mac.
     * @tag LT_VULKAN:
     *    @brief Vulkan
     *    @note available for all desktops but Mac
     **/
    enum class GRAPHIC_API : uint8 {
        OPENGL,
        VULKAN
    };

    /**
     * @struct Rect
     * @brief this struct represent this thing.
     * @field x:
     *	@type int
     *	@brief left most x/width coordinate.
     * @field y:
     *	@type int
     *	@brief lower most y/height coordinate.
     * @field width:
     *	@type int
     *	@brief right most x/width coordinate.
     * @field height:
     *	@type int
     *	@brief upper most y/height coordinate.
     **/
    struct Rect {
        int x;
        int y;
        int width;
        int height;
    };

    /**
     * @struct ColorRGBA8
     * @brief RGBA Color with 8bits per channel.
     * @field r:
     *	@type uint8
     *	@brief red channel.
     * @field g:
     *	@type uint8
     *	@brief green channel.
     * @field b:
     *	@type uint8
     *	@brief blue channel.
     * @field a:
     *	@type uint8
     *	@brief alpha channel.
     * @field hash:
     *	@type uint32
     *	@brief hash identifier for the color.
     **/
    struct ColorRGBA8 {
        union {
            struct {
                uint8 r;
                uint8 g;
                uint8 b;
                uint8 a;
            };
            uint32 hash;
        };
    };

    /**
     * @struct ColorRGBA
     * @brief RGBA color with 32bits per channel.
     * @field r:
     *	@type float
     *	@brief red channel.
     * @field g:
     *	@type float
     *	@brief green channel.
     * @field b:
     *	@type float
     *	@brief blue channel.
     * @field a:
     *	@type float
     *	@brief alpha channel.
     **/
    struct ColorRGBA {
        float r;
        float g;
        float b;
        float a;
    };

    /**
     * @enum BufferBit
     * @brief Available buffers in a render target.
     * @tag LT_COLOR_BIT
     *    @brief Color buffer
     * @tag LT_DEPTH_BIT
     *    @brief Depth buffer
     * @tag LT_STENCIL_BIT
     *    @brief Stencil buffer
     **/
    enum class BUFFER_BIT : uint8 {
        LT_COLOR_BIT = 1,
        LT_DEPTH_BIT = 2,
        LT_STENCIL_BIT = 4
    };

    /**
     * @struct GraphicsAPI
     * @brief Access to APIs functions.
     **/
    struct GraphicsAPI {
        void (*clearScreenColor8)(const ColorRGBA8 in_value);
        void (*clearScreenColor)(const ColorRGBA* in_value);
        void (*clearScreen)(const BUFFER_BIT in_value);
        void (*enableAlphaBlending)(const uint8 in_value);
        void (*enableDepthTesting)(const uint8 in_value);
        void (*enableStencilTesting)(const uint8 in_value);
        void (*enableScissorTesting)(const uint8 in_value);
        void (*setScissor)(const Rect* in_value);
        void (*setViewport)(const Rect* in_value);
        void (*swapBuffers)(void);
    };

    extern GraphicsAPI api;

    void GraphicsAPI_Init(const GRAPHIC_API in_api);
}