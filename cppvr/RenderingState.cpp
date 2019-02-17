#include "RenderingState.h"

using namespace ej;

RenderingState::RenderingState(const Core& core)
{
}

void RenderingState::apply()
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
}

void RenderingState::setViewport(const glm::ivec2& size, const glm::ivec2& offset)
{
	setViewport(offset.x, offset.y, size.x, size.y);
}

void RenderingState::setViewport(GLint x, GLint y, GLsizei width, GLsizei height)
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

glm::ivec2 RenderingState::getViewportSize() const
{
	return glm::ivec2(m_viewport[2], m_viewport[3]);
}

glm::ivec2 RenderingState::getViewportOffset() const
{
	return glm::ivec2(m_viewport[0], m_viewport[1]);
}

void RenderingState::setClearColor(const glm::vec4& color)
{
	if (!std::memcmp(&color[0], &m_clearColor[0], sizeof(GLclampf) * 4)) {
		std::memcpy(&m_clearColor[0], &color[0], sizeof(GLclampf) * 4);

		glClearColor(color.r, color.g, color.b, color.a);
	}
}

void RenderingState::setClearColor(float r, float g, float b, float a)
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

glm::vec4 RenderingState::getClearColor() const
{
	return glm::vec4(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
}

void RenderingState::setClearDepth(float depth)
{
	if (m_clearDepth != depth) {
		m_clearDepth = depth;
		glClearDepth(depth);
	}
}

float RenderingState::getClearDepth() const
{
	return m_clearDepth;
}

void RenderingState::setDepthTestEnabled(bool enabled)
{
	if (m_isDepthTestEnabled != enabled) {
		m_isDepthTestEnabled = enabled;
		setEnabled(GL_DEPTH_TEST, enabled);
	}
}

bool RenderingState::isDepthTestEnabled() const
{
	return m_isDepthTestEnabled;
}

void RenderingState::setDepthWriteEnabled(bool enabled)
{
	if (m_isDepthWriteEnabled != enabled) {
		m_isDepthWriteEnabled = enabled;
		glDepthMask(static_cast<GLboolean>(enabled));
	}
}

bool RenderingState::isDepthWriteEnabled() const
{
	return m_isDepthWriteEnabled;
}

void RenderingState::setDepthTestFunction(GLenum testFunction)
{
	if (m_depthTestFunction != testFunction) {
		m_depthTestFunction = testFunction;
		glDepthFunc(testFunction);
	}
}

GLenum RenderingState::getDepthTestFunction() const
{
	return m_depthTestFunction;
}

void RenderingState::setClipControl(GLenum origin, GLenum depth)
{
	if (m_clipControlOrigin != origin || m_clipControlDepth != depth) {
		m_clipControlOrigin = origin;
		m_clipControlDepth = depth;
		glClipControl(origin, depth);
	}
}

GLenum RenderingState::getClipControlOrigin() const
{
	return m_clipControlOrigin;
}

GLenum RenderingState::getClipControlDepth() const
{
	return m_clipControlDepth;
}

void RenderingState::setBlendingEnabled(bool enabled)
{
	if (m_isBlendingEnabled != enabled) {
		m_isBlendingEnabled = enabled;
		setEnabled(GL_BLEND, enabled);
	}
}

bool RenderingState::isBlendingEnabled() const
{
	return m_isBlendingEnabled;
}

void RenderingState::setBlendingFunction(GLenum src, GLenum dst)
{
	if (m_blendingFunctionSrc != src ||
		m_blendingFunctionDst != dst)
	{
		m_blendingFunctionSrc = src;
		m_blendingFunctionDst = dst;
		glBlendFunc(src, dst);
	}
}

GLenum RenderingState::getBlendingFunctionSrc() const
{
	return m_blendingFunctionSrc;
}

GLenum RenderingState::getBlendingFunctionDst() const
{
	return m_blendingFunctionDst;
}

void RenderingState::setFaceCullingEnabled(bool enabled)
{
	if (m_isFaceCullingEnabled != enabled) {
		m_isFaceCullingEnabled = enabled;
		setEnabled(GL_CULL_FACE, enabled);
	}
}

bool RenderingState::isFaceCullingEnabled() const
{
	return m_isFaceCullingEnabled;
}

void RenderingState::setFaceCullingSide(GLenum side)
{
	if (m_faceCullingSide != side) {
		m_faceCullingSide = side;
		glCullFace(side);
	}
}

GLenum RenderingState::getFaceCullingSide() const
{
	return m_faceCullingSide;
}

void RenderingState::setPolygonMode(GLenum mode)
{
	if (m_polygonMode != mode) {
		m_polygonMode = mode;
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}
}

GLenum RenderingState::getPolygonMode() const
{
	return m_polygonMode;
}

void RenderingState::setActiveTexture(size_t unit)
{
	if (unit != m_activeTextureUnit) {
		m_activeTextureUnit = unit;
		glActiveTexture(GL_TEXTURE0 + unit);
	}
}

void RenderingState::bindTexture(const Texture* texture, size_t unit)
{
	if (texture != nullptr) {
		setActiveTexture(unit);
		glBindTexture(texture->getTarget(), texture->getHandle());
	}
}

void RenderingState::setCurrentShader(Shader* shader)
{
	if (m_currentShader != shader) {
		m_currentShader = shader;
		if (shader == nullptr) {
			glUseProgram(0);
		}
		else {
			glUseProgram(shader->getHandle());
		}
	}
}

const Shader* RenderingState::getCurrentShader() const
{
	return m_currentShader;
}

void RenderingState::setCurrentFrameBuffer(FrameBuffer* frameBuffer)
{
	if (m_currentFrameBuffer != frameBuffer) {
		m_currentFrameBuffer = frameBuffer;
		if (frameBuffer == nullptr) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->getHandle());
		}
	}
}

const FrameBuffer* RenderingState::getCurrentFrameBuffer() const
{
	return m_currentFrameBuffer;
}

void RenderingState::setEnabled(GLenum prop, bool enabled)
{
	if (enabled) {
		glEnable(prop);
	}
	else {
		glDisable(prop);
	}
}
