#include "Managers/RenderingManager.h"

#include "Core/Core.h"
#include "Managers/WindowManager.h"

static void setEnabled(const GLenum prop, const bool enabled)
{
	if (enabled) {
		glEnable(prop);
	}
	else {
		glDisable(prop);
	}
}

ej::RenderingManager::RenderingManager(const Core& core) :
	BaseManager(core)
{
	m_windowManager = core.get<WindowManager>();

	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Unable to initialize GLEW");
	}
}

void ej::RenderingManager::synchronize()
{
	// depth test
	setEnabled(GL_DEPTH_TEST, m_renderingParameters.isDepthTestEnabled);

	// depth write
	glDepthMask(static_cast<GLboolean>(m_renderingParameters.isDepthWriteEnabled));

	// depth test function
	glDepthFunc(m_renderingParameters.depthTestFunction);

	// set clip control
	glClipControl(m_renderingParameters.clipControlOrigin, 
		m_renderingParameters.clipControlDepth);

	// set blending enabled
	setEnabled(GL_BLEND, m_renderingParameters.isBlendingEnabled);

	// set blending functions
	glBlendFunc(m_renderingParameters.blendingFunctionSrc, 
		m_renderingParameters.blendingFunctionDst);

	// set face culling enabled
	setEnabled(GL_CULL_FACE, m_renderingParameters.isFaceCullingEnabled);

	// set face culling side
	glCullFace(m_renderingParameters.faceCullingSide);

	// set polygon mode
	glPolygonMode(GL_FRONT_AND_BACK, m_renderingParameters.polygonMode);

	// viewport
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

	// clear color
	glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);

	// clear depth
	glClearDepth(m_clearDepth);

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

void ej::RenderingManager::apply(const RenderingParameters& parameters)
{
	setDepthTestEnabled(parameters.isDepthTestEnabled);
	setDepthWriteEnabled(parameters.isDepthWriteEnabled);
	setDepthTestFunction(parameters.depthTestFunction);
	setClipControl(parameters.clipControlOrigin, parameters.clipControlDepth);
	setBlendingEnabled(parameters.isBlendingEnabled);
	setBlendingFunctions(parameters.blendingFunctionSrc, parameters.blendingFunctionDst);
	setFaceCullingEnabled(parameters.isFaceCullingEnabled);
	setFaceCullingSide(parameters.faceCullingSide);
	setPolygonMode(parameters.polygonMode);
}

void ej::RenderingManager::draw()
{
	for (const auto& renderer : m_renderers) {
		renderer->draw();
	}
}

void ej::RenderingManager::setDepthTestEnabled(const bool enabled)
{
	if (m_renderingParameters.isDepthTestEnabled != enabled) {
		m_renderingParameters.isDepthTestEnabled = enabled;
		setEnabled(GL_DEPTH_TEST, enabled);
	}
}

bool ej::RenderingManager::isDepthTestEnabled() const
{
	return m_renderingParameters.isDepthTestEnabled;
}

void ej::RenderingManager::setDepthWriteEnabled(const bool enabled)
{
	if (m_renderingParameters.isDepthWriteEnabled != enabled) {
		m_renderingParameters.isDepthWriteEnabled = enabled;
		glDepthMask(static_cast<GLboolean>(enabled));
	}
}

bool ej::RenderingManager::isDepthWriteEnabled() const
{
	return m_renderingParameters.isDepthWriteEnabled;
}

void ej::RenderingManager::setDepthTestFunction(const GLenum testFunction)
{
	if (m_renderingParameters.depthTestFunction != testFunction) {
		m_renderingParameters.depthTestFunction = testFunction;
		glDepthFunc(testFunction);
	}
}

GLenum ej::RenderingManager::getDepthTestFunction() const
{
	return m_renderingParameters.depthTestFunction;
}

void ej::RenderingManager::setClipControl(const GLenum origin, const GLenum depth)
{
	if (m_renderingParameters.clipControlOrigin != origin || 
		m_renderingParameters.clipControlDepth != depth) 
	{
		m_renderingParameters.clipControlOrigin = origin;
		m_renderingParameters.clipControlDepth = depth;
		glClipControl(origin, depth);
	}
}

GLenum ej::RenderingManager::getClipControlOrigin() const
{
	return m_renderingParameters.clipControlOrigin;
}

GLenum ej::RenderingManager::getClipControlDepth() const
{
	return m_renderingParameters.clipControlDepth;
}

void ej::RenderingManager::setBlendingEnabled(const bool enabled)
{
	if (m_renderingParameters.isBlendingEnabled != enabled) {
		m_renderingParameters.isBlendingEnabled = enabled;
		setEnabled(GL_BLEND, enabled);
	}
}

bool ej::RenderingManager::isBlendingEnabled() const
{
	return m_renderingParameters.isBlendingEnabled;
}

void ej::RenderingManager::setBlendingFunctions(const GLenum src, const GLenum dst)
{
	if (m_renderingParameters.blendingFunctionSrc != src ||
		m_renderingParameters.blendingFunctionDst != dst)
	{
		m_renderingParameters.blendingFunctionSrc = src;
		m_renderingParameters.blendingFunctionDst = dst;
		glBlendFunc(src, dst);
	}
}

GLenum ej::RenderingManager::getBlendingFunctionSrc() const
{
	return m_renderingParameters.blendingFunctionSrc;
}

GLenum ej::RenderingManager::getBlendingFunctionDst() const
{
	return m_renderingParameters.blendingFunctionDst;
}

void ej::RenderingManager::setFaceCullingEnabled(const bool enabled)
{
	if (m_renderingParameters.isFaceCullingEnabled != enabled) {
		m_renderingParameters.isFaceCullingEnabled = enabled;
		setEnabled(GL_CULL_FACE, enabled);
	}
}

bool ej::RenderingManager::isFaceCullingEnabled() const
{
	return m_renderingParameters.isFaceCullingEnabled;
}

void ej::RenderingManager::setFaceCullingSide(const GLenum side)
{
	if (m_renderingParameters.faceCullingSide != side) {
		m_renderingParameters.faceCullingSide = side;
		glCullFace(side);
	}
}

GLenum ej::RenderingManager::getFaceCullingSide() const
{
	return m_renderingParameters.faceCullingSide;
}

void ej::RenderingManager::setPolygonMode(const GLenum mode)
{
	if (m_renderingParameters.polygonMode != mode) {
		m_renderingParameters.polygonMode = mode;
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}
}

GLenum ej::RenderingManager::getPolygonMode() const
{
	return m_renderingParameters.polygonMode;
}

const ej::RenderingParameters& ej::RenderingManager::getRenderingParameters() const
{
	return m_renderingParameters;
}

void ej::RenderingManager::setViewport(const glm::ivec2& size, const glm::ivec2& offset)
{
	setViewport(offset.x, offset.y, size.x, size.y);
}

void ej::RenderingManager::setViewport(GLint x, GLint y, GLsizei width, GLsizei height)
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

glm::ivec2 ej::RenderingManager::getViewportSize() const
{
	return glm::ivec2(m_viewport[2], m_viewport[3]);
}

glm::ivec2 ej::RenderingManager::getViewportOffset() const
{
	return glm::ivec2(m_viewport[0], m_viewport[1]);
}

void ej::RenderingManager::setClearColor(const glm::vec4& color)
{
	if (!std::memcmp(&color[0], &m_clearColor[0], sizeof(GLclampf) * 4)) {
		std::memcpy(&m_clearColor[0], &color[0], sizeof(GLclampf) * 4);

		glClearColor(color.r, color.g, color.b, color.a);
	}
}

void ej::RenderingManager::setClearColor(const sf::Color& color)
{
	setClearColor(
		static_cast<float>(color.r) / 255.0f,
		static_cast<float>(color.g) / 255.0f,
		static_cast<float>(color.b) / 255.0f,
		static_cast<float>(color.a) / 255.0f
	);
}

void ej::RenderingManager::setClearColor(float r, float g, float b, float a)
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

glm::vec4 ej::RenderingManager::getClearColor() const
{
	return glm::vec4(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
}

void ej::RenderingManager::setClearDepth(const float depth)
{
	if (m_clearDepth != depth) {
		m_clearDepth = depth;
		glClearDepth(depth);
	}
}

float ej::RenderingManager::getClearDepth() const
{
	return m_clearDepth;
}

void ej::RenderingManager::setCurrentShader(Shader * shader)
{
	GLuint shaderId = 0;
	if (shader != nullptr) {
		shaderId = shader->getHandle();
	}

	setCurrentShaderId(shaderId);
}

void ej::RenderingManager::setCurrentShaderId(const GLuint shaderId)
{
	if (m_currentShaderId != shaderId) {
		m_currentShaderId = shaderId;
		glUseProgram(shaderId);
	}
}

GLuint ej::RenderingManager::getCurrentShaderId() const
{
	return m_currentShaderId;
}

void ej::RenderingManager::setCurrentFrameBuffer(FrameBuffer * frameBuffer)
{
	GLuint frameBufferId = 0;
	if (frameBuffer != nullptr) {
		frameBufferId = frameBuffer->getHandle();
	}

	setCurrentFrameBufferId(frameBufferId);
}

void ej::RenderingManager::setCurrentFrameBufferId(const GLuint frameBufferId)
{
	if (m_currentFrameBufferId != frameBufferId) {
		m_currentFrameBufferId = frameBufferId;
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	}
}

GLuint ej::RenderingManager::getCurrentFrameBufferId() const
{
	return m_currentFrameBufferId;
}

void ej::RenderingManager::setActiveTexture(const unsigned int unit)
{
	if (unit != m_activeTextureUnit) {
		m_activeTextureUnit = unit;
		glActiveTexture(GL_TEXTURE0 + unit);
	}
}

void ej::RenderingManager::bindTexture(const Texture * texture, const unsigned int unit)
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

void ej::RenderingManager::bindTexture(const GLenum textureTarget, const GLuint textureId, const unsigned int unit)
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
