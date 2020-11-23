#pragma once
#include <Common.hpp>
#include "GraphicsAPI.hpp"

namespace LT {
	void GL_ClearScreenColor8(const ColorRGBA8 in_value);

	void GL_ClearScreenColor(const ColorRGBA* in_value);

	void GL_ClearScreen(const BUFFER_BIT in_value);

	void GL_EnableDepthTesting(const bool in_value);

	void GL_EnableStencilTesting(const bool in_value);

	void GL_EnableAlphaBlending(const bool in_value);

	void GL_EnableScissorTesting(const bool in_value);

	void GL_SetScissor(const Rect* in_value);

	void GL_SetViewport(const Rect* in_value);
}