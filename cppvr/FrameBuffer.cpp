#include "FrameBuffer.h"
#include <stdexcept>

using namespace ej;

FrameBuffer::FrameBuffer() :
	m_isInitialized(false)
{
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_id);
	glDeleteRenderbuffers(1, &m_depthBuffer);
}

Texture & FrameBuffer::getColorTexture()
{
	return m_colorTexture;
}

void FrameBuffer::init(unsigned int width, unsigned int height, bool depthEnabled)
{
	if (m_isInitialized) {
		return;
	}

	if (!m_colorTexture.init(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, nullptr)) {
		throw std::runtime_error("Unable to create frame buffer color texture");
	}

	if (depthEnabled) {
		glGenRenderbuffers(1, &m_depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	glGenFramebuffers(1, &m_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture.getHandle(), 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_isInitialized = true;
}