#pragma once

#include <GL/glew.h>

namespace ej
{
	struct RenderingParameters final
	{
		bool isDepthTestEnabled = true;
		bool isDepthWriteEnabled = true;
		GLenum depthTestFunction = GL_LEQUAL;

		GLenum clipControlOrigin = GL_LOWER_LEFT;
		GLenum clipControlDepth = GL_NEGATIVE_ONE_TO_ONE;

		bool isBlendingEnabled = false;
		GLenum blendingFunctionSrc = GL_SRC_ALPHA;
		GLenum blendingFunctionDst = GL_ONE_MINUS_SRC_ALPHA;

		bool isFaceCullingEnabled = true;
		GLenum faceCullingSide = GL_BACK;

		GLenum polygonMode = GL_FILL;
	};
}
