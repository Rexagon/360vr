#pragma once

#include <GL/glew.h>

#include "Core/PointerDefs.h"
#include "Resources/Texture.h"

namespace ej 
{
	class FrameBuffer : public PointerDefs<FrameBuffer>
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		void init(unsigned int width, unsigned int height, bool depthEnabled);

		Texture& getColorTexture();

		GLuint getHandle() const { return m_id; }

	private:
		bool m_isInitialized;
		GLuint m_id;

		Texture m_colorTexture;
		GLuint m_depthBuffer;
	};
}
