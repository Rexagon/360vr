#include "Rendering/TextureStreamer.h"

TextureStreamer::TextureStreamer() :
	m_buffers{0}, m_currentBufferIndex(0)
{
	glGenBuffers(2, m_buffers);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_buffers[0]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, 800 * 600 * 3, nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_buffers[1]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, 800 * 600 * 3, nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

TextureStreamer::~TextureStreamer()
{
	glDeleteBuffers(2, m_buffers);
}

void TextureStreamer::write(ej::Texture* texture, Video* video)
{
	if (texture == nullptr || video == nullptr) {
		return;
	}

	const glm::ivec2 videoSize = video->getSize();
	if (glm::ivec2(texture->getSize()) != videoSize) {
		texture->resize(videoSize.x, videoSize.y);
	}

	size_t dataSize = videoSize.x * videoSize.y * 3;

	m_currentBufferIndex = (m_currentBufferIndex + 1) % 2;
	const auto nextBufferIndex = (m_currentBufferIndex + 1) % 2;

	glBindTexture(GL_TEXTURE_2D, texture->getHandle());

	// Nth PBO
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_buffers[m_currentBufferIndex]);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, videoSize.x, videoSize.y, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	// N+1 PBO
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_buffers[nextBufferIndex]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, dataSize, nullptr, GL_STREAM_DRAW);

	// Write data
	const auto ptr = static_cast<GLubyte*>(glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY));
	if (ptr) {
		video->writeVideoData(ptr, dataSize);
		//std::memset(ptr, rand() % 255, m_dataSize);
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	}

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}
