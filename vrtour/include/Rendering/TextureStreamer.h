#pragma once

#include <GL/glew.h>

#include <Core/PointerDefs.h>
#include <Resources/Texture.h>

#include "Video/Video.h"

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
