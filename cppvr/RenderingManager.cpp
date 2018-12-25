#include "RenderingManager.h"

#include <GL/glew.h>

#include "WindowManager.h"

using namespace ej;

RenderingManager::RenderingManager(const Core& core) :
	BaseManager(core),
	m_viewport{0}, m_clearColor{0.0f, 0.0f, 0.0f, 1.0f}, m_clearDepth(1.0f),
	m_isDepthTestEnabled(true), m_isDepthWriteEnabled(true), m_depthTestFunction(GL_LEQUAL),
	m_clipControlOrigin(GL_LOWER_LEFT), m_clipControlDepth(GL_NEGATIVE_ONE_TO_ONE),
	m_isBlendingEnabled(false), m_blendingFunctionSrc(GL_SRC_ALPHA), m_blendingFunctionDst(GL_ONE_MINUS_SRC_ALPHA),
	m_isFaceCullingEnabled(true), m_faceCullingSide(GL_BACK), m_polygonMode(GL_FILL), 
	m_currentShader(nullptr), m_currentFrameBuffer(nullptr)
{
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Unable to initialize GLEW");
	}
}

void RenderingManager::apply()
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

void RenderingManager::setViewport(const glm::ivec2& size, const glm::ivec2& offset)
{
	setViewport(offset.x, offset.y, size.x, size.y);
}

void RenderingManager::setViewport(GLint x, GLint y, GLsizei width, GLsizei height)
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

glm::ivec2 RenderingManager::getViewportSize() const
{
	return glm::ivec2(m_viewport[2], m_viewport[3]);
}

glm::ivec2 RenderingManager::getViewportOffset() const
{
	return glm::ivec2(m_viewport[0], m_viewport[1]);
}

void RenderingManager::setClearColor(const glm::vec4& color)
{
	if (!std::memcmp(&color[0], &m_clearColor[0], sizeof(GLclampf) * 4)) {
		std::memcpy(&m_clearColor[0], &color[0], sizeof(GLclampf) * 4);

		glClearColor(color.r, color.g, color.b, color.a);
	}
}

void RenderingManager::setClearColor(float r, float g, float b, float a)
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

glm::vec4 RenderingManager::getClearColor() const
{
	return glm::vec4(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
}

void RenderingManager::setClearDepth(float depth)
{
	if (m_clearDepth != depth) {
		m_clearDepth = depth;
		glClearDepth(depth);
	}
}

float RenderingManager::getClearDepth() const
{
	return m_clearDepth;
}

void RenderingManager::setDepthTestEnabled(bool enabled)
{
	if (m_isDepthTestEnabled != enabled) {
		m_isDepthTestEnabled = enabled;
		setEnabled(GL_DEPTH_TEST, enabled);
	}
}

bool RenderingManager::isDepthTestEnabled() const
{
	return m_isDepthTestEnabled;
}

void RenderingManager::setDepthWriteEnabled(bool enabled)
{
	if (m_isDepthWriteEnabled != enabled) {
		m_isDepthWriteEnabled = enabled;
		glDepthMask(static_cast<GLboolean>(enabled));
	}
}

bool RenderingManager::isDepthWriteEnabled() const
{
	return m_isDepthWriteEnabled;
}

void RenderingManager::setDepthTestFunction(GLenum testFunction)
{
	if (m_depthTestFunction != testFunction) {
		m_depthTestFunction = testFunction;
		glDepthFunc(testFunction);
	}
}

GLenum RenderingManager::getDepthTestFunction() const
{
	return m_depthTestFunction;
}

void RenderingManager::setClipControl(GLenum origin, GLenum depth)
{
	if (m_clipControlOrigin != origin || m_clipControlDepth != depth) {
		m_clipControlOrigin = origin;
		m_clipControlDepth = depth;
		glClipControl(origin, depth);
	}
}

GLenum RenderingManager::getClipControlOrigin() const
{
	return m_clipControlOrigin;
}

GLenum RenderingManager::getClipControlDepth() const
{
	return m_clipControlDepth;
}

void RenderingManager::setBlendingEnabled(bool enabled)
{
	if (m_isBlendingEnabled != enabled) {
		m_isBlendingEnabled = enabled;
		setEnabled(GL_BLEND, enabled);
	}
}

bool RenderingManager::isBlendingEnabled() const
{
	return m_isBlendingEnabled;
}

void RenderingManager::setBlendingFunction(GLenum src, GLenum dst)
{
	if (m_blendingFunctionSrc != src ||
		m_blendingFunctionDst != dst)
	{
		m_blendingFunctionSrc = src;
		m_blendingFunctionDst = dst;
		glBlendFunc(src, dst);
	}
}

GLenum RenderingManager::getBlendingFunctionSrc() const
{
	return m_blendingFunctionSrc;
}

GLenum RenderingManager::getBlendingFunctionDst() const
{
	return m_blendingFunctionDst;
}

void RenderingManager::setFaceCullingEnabled(bool enabled)
{
	if (m_isFaceCullingEnabled != enabled) {
		m_isFaceCullingEnabled = enabled;
		setEnabled(GL_CULL_FACE, enabled);
	}
}

bool RenderingManager::isFaceCullingEnabled() const
{
	return m_isFaceCullingEnabled;
}

void RenderingManager::setFaceCullingSide(GLenum side)
{
	if (m_faceCullingSide != side) {
		m_faceCullingSide = side;
		glCullFace(side);
	}
}

GLenum RenderingManager::getFaceCullingSide() const
{
	return m_faceCullingSide;
}

void RenderingManager::setPolygonMode(GLenum mode)
{
	if (m_polygonMode != mode) {
		m_polygonMode = mode;
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}
}

GLenum RenderingManager::getPolygonMode() const
{
	return m_polygonMode;
}

void RenderingManager::setCurrentShader(const Shader* shader)
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

const Shader* RenderingManager::getCurrentShader() const
{
	return m_currentShader;
}

void RenderingManager::setCurrentFrameBuffer(const FrameBuffer* frameBuffer)
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

const FrameBuffer* RenderingManager::getCurrentFrameBuffer() const
{
	return m_currentFrameBuffer;
}

void RenderingManager::setEnabled(GLenum prop, bool enabled)
{
	if (enabled) {
		glEnable(prop);
	}
	else {
		glDisable(prop);
	}
}


