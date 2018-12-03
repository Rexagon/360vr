#pragma once

#include <GL/glew.h>
#include <glm/vec2.hpp>

#include "Texture.h"

namespace ej {
	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		void init(unsigned int width, unsigned int height, bool depthEnabled);

		void bind() const;
		static void unbind();

		Texture& getColorTexture();

		GLuint getHandle() const { return m_id; }

	private:
		bool m_isInitialized;
		GLuint m_id;

		Texture m_colorTexture;
		GLuint m_depthBuffer;
	};
}