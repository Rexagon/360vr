#pragma once

#include <GL/glew.h>

#include <Resources/Texture.h>

#include "Video/VideoStream.h"

class TextureStreamer final
{
public:
	TextureStreamer();
	~TextureStreamer();

	void write(ej::Texture* texture, VideoStream* stream);

private:
	uint64_t m_decodingId{ 0 };

	GLuint m_buffers[2]{ 0 };
	size_t m_currentBufferIndex{ 0 };
};
