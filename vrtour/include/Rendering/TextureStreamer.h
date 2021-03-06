#pragma once

#include <GL/glew.h>

#include <Resources/Texture.h>

#include "Video/VideoStream.h"

namespace ej 
{
	class RenderingManager;
}

namespace app
{
	class TextureStreamer final
	{
	public:
		explicit TextureStreamer(const ej::Core& core);
		~TextureStreamer();

		void init();

		void write(ej::Texture* texture, VideoStream* stream);

		bool isInitialized() const;

	private:
		static const unsigned DEFAULT_TEXTURE_UNIT = 14;

		ej::RenderingManager* m_renderingManager = nullptr;

		bool m_isInitialized = false;

		uint64_t m_decodingId = 0;

		GLuint m_buffers[2]{ 0 };
		size_t m_currentBufferIndex = 0;
	};
}
