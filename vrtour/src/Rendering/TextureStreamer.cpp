#include "Rendering/TextureStreamer.h"

app::TextureStreamer::TextureStreamer()
{
	glGenBuffers(2, m_buffers);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_buffers[0]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, 800 * 600 * 3, nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_buffers[1]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, 800 * 600 * 3, nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

app::TextureStreamer::~TextureStreamer()
{
	glDeleteBuffers(2, m_buffers);
}

void app::TextureStreamer::write(ej::Texture* texture, VideoStream* stream)
{
	//TODO: fix rendering state resynchronization

	if (texture == nullptr || stream == nullptr || 
		stream->getCurrentDecodingId() == m_decodingId) 
	{
		return;
	}

	const glm::ivec2 videoSize = stream->getSize();
	if (glm::ivec2(texture->getSize()) != videoSize) {
		texture->resize(videoSize.x, videoSize.y);
	}

	m_currentBufferIndex = (m_currentBufferIndex + 1) % 2;
	const auto nextBufferIndex = (m_currentBufferIndex + 1) % 2;

	glBindTexture(GL_TEXTURE_2D, texture->getHandle());

	const auto size = stream->getBufferSize();

	// Nth PBO
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_buffers[m_currentBufferIndex]);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, videoSize.x, videoSize.y, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	// N+1 PBO
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_buffers[nextBufferIndex]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, size, nullptr, GL_STREAM_DRAW);

	// Write data
	const auto ptr = static_cast<GLubyte*>(glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY));
	if (ptr) {
		stream->writeVideoData(ptr, size, m_decodingId);
		m_decodingId = stream->getCurrentDecodingId();
		//std::memset(ptr, rand() % 255, m_dataSize);
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	}

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}
