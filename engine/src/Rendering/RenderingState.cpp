#include "Rendering/RenderingState.h"

static void setEnabled(const GLenum prop, const bool enabled)
{
	if (enabled) {
		glEnable(prop);
	}
	else {
		glDisable(prop);
	}
}

void ej::RenderingState::apply()
{
	// viewport
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

	// clear color
	glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);

	// clear depth
	glClearDepth(m_clearDepth);

	// depth test
	setEnabled(GL_DEPTH_TEST, m_isDepthTestEnabled);

	// depth write
	glDepthMask(static_cast<GLboolean>(m_isDepthWriteEnabled));

	// depth test function
	glDepthFunc(m_depthTestFunction);

	// set clip control
	glClipControl(m_clipControlOrigin, m_clipControlDepth);

	// set blending enabled
	setEnabled(GL_BLEND, m_isBlendingEnabled);

	// set blending functions
	glBlendFunc(m_blendingFunctionSrc, m_blendingFunctionDst);

	// set face culling enabled
	setEnabled(GL_CULL_FACE, m_isFaceCullingEnabled);

	// set face culling side
	glCullFace(m_faceCullingSide);

	// set polygon mode
	glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);

	// set current shader
	glUseProgram(m_currentShaderId);

	// set current frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_currentFrameBufferId);

	for (size_t i = 0; i < TEXTURE_COUNT; ++i) {
		const auto& texture = m_currentTextures[i];
		glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(i));
		glBindTexture(texture.first, texture.second);
	}

	// set active texture unit
	glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(m_activeTextureUnit));
}

void ej::RenderingState::setViewport(const glm::ivec2& size, const glm::ivec2& offset)
{
	setViewport(offset.x, offset.y, size.x, size.y);
}

void ej::RenderingState::setViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	if (m_viewport[0] != x ||
		m_viewport[1] != y ||
		m_viewport[2] != width ||
		m_viewport[3] != height)
	{
		m_viewport[0] = x;
		m_viewport[1] = y;
		m_viewport[2] = width;
		m_viewport[3] = height;
		glViewport(x, y, width, height);
	}
}

glm::ivec2 ej::RenderingState::getViewportSize() const
{
	return glm::ivec2(m_viewport[2], m_viewport[3]);
}

glm::ivec2 ej::RenderingState::getViewportOffset() const
{
	return glm::ivec2(m_viewport[0], m_viewport[1]);
}

void ej::RenderingState::setClearColor(const glm::vec4& color)
{
	if (!std::memcmp(&color[0], &m_clearColor[0], sizeof(GLclampf) * 4)) {
		std::memcpy(&m_clearColor[0], &color[0], sizeof(GLclampf) * 4);

		glClearColor(color.r, color.g, color.b, color.a);
	}
}

void ej::RenderingState::setClearColor(const sf::Color& color)
{
	setClearColor(
		static_cast<float>(color.r) / 255.0f,
		static_cast<float>(color.g) / 255.0f,
		static_cast<float>(color.b) / 255.0f,
		static_cast<float>(color.a) / 255.0f
	);
}

void ej::RenderingState::setClearColor(float r, float g, float b, float a)
{
	if (m_clearColor[0] != r ||
		m_clearColor[1] != g ||
		m_clearColor[2] != b ||
		m_clearColor[3] != a)
	{
		m_clearColor[0] = r;
		m_clearColor[1] = g;
		m_clearColor[2] = b;
		m_clearColor[3] = a;

		glClearColor(r, g, b, a);
	}
}

glm::vec4 ej::RenderingState::getClearColor() const
{
	return glm::vec4(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
}

void ej::RenderingState::setClearDepth(const float depth)
{
	if (m_clearDepth != depth) {
		m_clearDepth = depth;
		glClearDepth(depth);
	}
}

float ej::RenderingState::getClearDepth() const
{
	return m_clearDepth;
}

void ej::RenderingState::setDepthTestEnabled(const bool enabled)
{
	if (m_isDepthTestEnabled != enabled) {
		m_isDepthTestEnabled = enabled;
		setEnabled(GL_DEPTH_TEST, enabled);
	}
}

bool ej::RenderingState::isDepthTestEnabled() const
{
	return m_isDepthTestEnabled;
}

void ej::RenderingState::setDepthWriteEnabled(const bool enabled)
{
	if (m_isDepthWriteEnabled != enabled) {
		m_isDepthWriteEnabled = enabled;
		glDepthMask(static_cast<GLboolean>(enabled));
	}
}

bool ej::RenderingState::isDepthWriteEnabled() const
{
	return m_isDepthWriteEnabled;
}

void ej::RenderingState::setDepthTestFunction(const GLenum testFunction)
{
	if (m_depthTestFunction != testFunction) {
		m_depthTestFunction = testFunction;
		glDepthFunc(testFunction);
	}
}

GLenum ej::RenderingState::getDepthTestFunction() const
{
	return m_depthTestFunction;
}

void ej::RenderingState::setClipControl(const GLenum origin, const GLenum depth)
{
	if (m_clipControlOrigin != origin || m_clipControlDepth != depth) {
		m_clipControlOrigin = origin;
		m_clipControlDepth = depth;
		glClipControl(origin, depth);
	}
}

GLenum ej::RenderingState::getClipControlOrigin() const
{
	return m_clipControlOrigin;
}

GLenum ej::RenderingState::getClipControlDepth() const
{
	return m_clipControlDepth;
}

void ej::RenderingState::setBlendingEnabled(const bool enabled)
{
	if (m_isBlendingEnabled != enabled) {
		m_isBlendingEnabled = enabled;
		setEnabled(GL_BLEND, enabled);
	}
}

bool ej::RenderingState::isBlendingEnabled() const
{
	return m_isBlendingEnabled;
}

void ej::RenderingState::setBlendingFunctions(const GLenum src, const GLenum dst)
{
	if (m_blendingFunctionSrc != src ||
		m_blendingFunctionDst != dst)
	{
		m_blendingFunctionSrc = src;
		m_blendingFunctionDst = dst;
		glBlendFunc(src, dst);
	}
}

GLenum ej::RenderingState::getBlendingFunctionSrc() const
{
	return m_blendingFunctionSrc;
}

GLenum ej::RenderingState::getBlendingFunctionDst() const
{
	return m_blendingFunctionDst;
}

void ej::RenderingState::setFaceCullingEnabled(const bool enabled)
{
	if (m_isFaceCullingEnabled != enabled) {
		m_isFaceCullingEnabled = enabled;
		setEnabled(GL_CULL_FACE, enabled);
	}
}

bool ej::RenderingState::isFaceCullingEnabled() const
{
	return m_isFaceCullingEnabled;
}

void ej::RenderingState::setFaceCullingSide(const GLenum side)
{
	if (m_faceCullingSide != side) {
		m_faceCullingSide = side;
		glCullFace(side);
	}
}

GLenum ej::RenderingState::getFaceCullingSide() const
{
	return m_faceCullingSide;
}

void ej::RenderingState::setPolygonMode(const GLenum mode)
{
	if (m_polygonMode != mode) {
		m_polygonMode = mode;
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}
}

GLenum ej::RenderingState::getPolygonMode() const
{
	return m_polygonMode;
}

void ej::RenderingState::setActiveTexture(const unsigned int unit)
{
	if (unit != m_activeTextureUnit) {
		m_activeTextureUnit = unit;
		glActiveTexture(GL_TEXTURE0 + unit);
	}
}

void ej::RenderingState::bindTexture(const Texture* texture, const unsigned int unit)
{
	GLenum textureTarget = GL_TEXTURE_2D;
	GLuint textureId = 0;
	if (texture != nullptr) {
		textureTarget = texture->getTarget();
		textureId = texture->getHandle();
	}
	else if (unit < TEXTURE_COUNT) {
		// Don't change cached texture target for nullptr
		textureTarget = m_currentTextures[unit].first;
	}

	bindTexture(textureTarget, textureId, unit);
}

void ej::RenderingState::bindTexture(const GLenum textureTarget, const GLuint textureId, const unsigned int unit)
{
	if (unit < TEXTURE_COUNT) {
		if (textureTarget == m_currentTextures[unit].first &&
			textureId == m_currentTextures[unit].second)
		{
			return;
		}

		m_currentTextures[unit].first = textureTarget;
		m_currentTextures[unit].second = textureId;
	}

	setActiveTexture(unit);
	glBindTexture(textureTarget, textureId);
}

void ej::RenderingState::setCurrentShader(Shader* shader)
{
	GLuint shaderId = 0;
	if (shader != nullptr) {
		shaderId = shader->getHandle();
	}

	setCurrentShaderId(shaderId);
}

void ej::RenderingState::setCurrentShaderId(const GLuint shaderId)
{
	if (m_currentShaderId != shaderId) {
		m_currentShaderId = shaderId;
		glUseProgram(shaderId);
	}
}

GLuint ej::RenderingState::getCurrentShaderId() const
{
	return m_currentShaderId;
}

void ej::RenderingState::setCurrentFrameBuffer(FrameBuffer* frameBuffer)
{
	GLuint frameBufferId = 0;
	if (frameBuffer != nullptr) {
		frameBufferId = frameBuffer->getHandle();
	}

	setCurrentFrameBufferId(frameBufferId);
}

void ej::RenderingState::setCurrentFrameBufferId(const GLuint frameBufferId)
{
	if (m_currentFrameBufferId != frameBufferId) {
		m_currentFrameBufferId = frameBufferId;
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	}
}

GLuint ej::RenderingState::getCurrentFrameBufferId() const
{
	return m_currentFrameBufferId;
}
