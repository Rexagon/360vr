#pragma once

#include <GL/glew.h>

#include "Video.h"
#include "Texture.h"
#include "PointerDefs.h"

class TextureStreamer : public ej::PointerDefs<TextureStreamer>
{
public:
	TextureStreamer();
	~TextureStreamer();

	void write(ej::Texture* texture, Video* video);

private:
	GLuint m_buffers[2];
	size_t m_currentBufferIndex;
};
