#include "Rendering/FrameBuffer.h"

#include <stdexcept>
#include <Managers/RenderingManager.h>

ej::FrameBuffer::FrameBuffer(const Core& core) :
	m_colorTexture(core)
{
	m_renderingManager = core.get<RenderingManager>();
}

ej::FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_id);
	glDeleteRenderbuffers(1, &m_depthBuffer);
}

void ej::FrameBuffer::init(const unsigned int width, const unsigned int height, const bool depthEnabled)
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
	const auto lastFrameBufferId = m_renderingManager->getCurrentFrameBufferId();
	m_renderingManager->setCurrentFrameBufferId(m_id);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture.getHandle(), 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

	m_renderingManager->setCurrentFrameBufferId(lastFrameBufferId);

	m_isInitialized = true;
}

bool ej::FrameBuffer::isInitialized() const
{
	return m_isInitialized;
}

ej::Texture& ej::FrameBuffer::getColorTexture()
{
	return m_colorTexture;
}

GLuint ej::FrameBuffer::getHandle() const
{
	return m_id;
}
