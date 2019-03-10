#include "Resources/Texture.h"

#include "Managers/RenderingManager.h"

ej::Texture::Texture(const Core& core, const GLenum target) :
	m_target(target)
{
	m_renderingManager = core.get<RenderingManager>();
}

ej::Texture::~Texture()
{
	if (m_initialized) {
		glDeleteTextures(1, &m_id);
	}
}

bool ej::Texture::init(const unsigned int width, const GLenum internalFormat, 
	const GLenum format, const GLenum type, const void * data)
{
	if (m_initialized || m_target != GL_TEXTURE_1D || 
		width == 0) 
	{
		return false;
	}

	glGenTextures(1, &m_id);

	m_size = glm::ivec3(width, 0, 0);
	m_internalFormat = internalFormat;
	m_format = format;
	m_type = type;

	m_renderingManager->setActiveTexture(DEFAULT_TEXTURE_UNIT);
	m_renderingManager->bindTexture(m_target, m_id, DEFAULT_TEXTURE_UNIT);

	glTexImage1D(m_target, 0, internalFormat, static_cast<GLsizei>(width), 
		0, format, type, data);

	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_minFilter);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_maxFilter);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_wrapS);

	m_initialized = true;
	return true;
}

bool ej::Texture::init(const unsigned int width, const unsigned int height, 
	const GLenum internalFormat, const GLenum format, const GLenum type,
	const void * data)
{
	if (m_initialized || m_target != GL_TEXTURE_2D || 
		width == 0 || height == 0) 
	{
		return false;
	}

	glGenTextures(1, &m_id);

	m_size = glm::uvec3(width, height, 0);
	m_internalFormat = internalFormat;
	m_format = format;
	m_type = type;

	m_renderingManager->setActiveTexture(DEFAULT_TEXTURE_UNIT);
	m_renderingManager->bindTexture(m_target, m_id, DEFAULT_TEXTURE_UNIT);

	glTexImage2D(m_target, 0, internalFormat, static_cast<GLsizei>(width), 
		static_cast<GLsizei>(height), 0, format, type, data);

	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_minFilter);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_maxFilter);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_wrapS);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_wrapT);

	m_initialized = true;
	return true;
}

bool ej::Texture::init(unsigned int width, unsigned int height, unsigned int depth, 
	GLenum internalFormat, GLenum format, GLenum type, const void * data)
{
	if (m_initialized || m_target != GL_TEXTURE_3D || 
		width == 0 || height == 0 || depth == 0) 
	{
		return false;
	}

	glGenTextures(1, &m_id);

	m_size = glm::uvec3(width, height, depth);
	m_internalFormat = internalFormat;
	m_format = format;
	m_type = type;

	m_renderingManager->setActiveTexture(DEFAULT_TEXTURE_UNIT);
	m_renderingManager->bindTexture(m_target, m_id, DEFAULT_TEXTURE_UNIT);

	glTexImage3D(m_target, 0, internalFormat, static_cast<GLsizei>(width), 
		static_cast<GLsizei>(height), static_cast<GLsizei>(depth), 
		0, format, type, data);

	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_minFilter);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_maxFilter);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_wrapS);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_wrapT);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_R, m_wrapR);

	m_initialized = true;
	return true;
}

void ej::Texture::resize(const unsigned int width, const unsigned int height, 
	const unsigned int depth)
{
	if (!m_initialized) return;

	m_renderingManager->setActiveTexture(DEFAULT_TEXTURE_UNIT);
	m_renderingManager->bindTexture(m_target, m_id, DEFAULT_TEXTURE_UNIT);

	switch (m_target) {
		case GL_TEXTURE_1D:
			if (width == 0) return;
			m_size.x = width;
			glTexImage1D(GL_TEXTURE_1D, 0, m_internalFormat, 
				width, 0, m_format, m_type, nullptr);
			break;

		case GL_TEXTURE_2D:
			if (width == 0 || height == 0) return;
			m_size.x = width;
			m_size.y = height;
			glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, 
				width, height, 0, m_format, m_type, nullptr);
			break;

		case GL_TEXTURE_3D:
			if (width == 0 || height == 0 || depth == 0) return;
			m_size.x = width;
			m_size.y = height;
			m_size.z = height;
			glTexImage3D(GL_TEXTURE_3D, 0, m_internalFormat, 
				width, height, depth, 0, m_format, m_type, nullptr);
			break;

		default:
			break;
	}
}

void ej::Texture::setFilters(const GLenum minFilter, const GLenum maxFilter)
{
	if (m_minFilter == minFilter && m_maxFilter == maxFilter)  {
		return;
	}

	m_minFilter = minFilter;
	m_maxFilter = maxFilter;

	if (m_initialized) {
		m_renderingManager->setActiveTexture(DEFAULT_TEXTURE_UNIT);
		m_renderingManager->bindTexture(m_target, m_id, DEFAULT_TEXTURE_UNIT);

		if (m_minFilter != minFilter) {
			glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, minFilter);
		}

		if (m_maxFilter != maxFilter) {
			glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, maxFilter);
		}
	}
}

void ej::Texture::setWrapMode(const GLenum wrapMode)
{
	switch (m_target) {
		case GL_TEXTURE_3D:
			setWrapMode(wrapMode, 2);
		case GL_TEXTURE_2D:
			setWrapMode(wrapMode, 1);
		case GL_TEXTURE_1D:
			setWrapMode(wrapMode, 0);
		default:
			break;
	}
}

void ej::Texture::setWrapMode(const GLenum wrapMode, const size_t component)
{
	GLenum wrapComponent = 0;
	GLenum* wrapVariable = nullptr;
	switch (component) {
		case 0:
			wrapVariable = &m_wrapS;
			wrapComponent = GL_TEXTURE_WRAP_S;
			break;

		case 1:
			wrapVariable = &m_wrapT;
			wrapComponent = GL_TEXTURE_WRAP_T;
			break;

		case 2:
			wrapVariable = &m_wrapR;
			wrapComponent = GL_TEXTURE_WRAP_R;
			break;

		default:
			return;
	}

	if (*wrapVariable != wrapMode) {
		*wrapVariable = wrapMode;

		if (m_initialized) {
			m_renderingManager->setActiveTexture(DEFAULT_TEXTURE_UNIT);
			m_renderingManager->bindTexture(m_target, m_id, DEFAULT_TEXTURE_UNIT);
			glTexParameteri(m_target, wrapComponent, wrapMode);
		}
	}
}

void ej::Texture::generateMipmap() const
{
	if (!m_initialized) {
		return;
	}

	m_renderingManager->setActiveTexture(DEFAULT_TEXTURE_UNIT);
	m_renderingManager->bindTexture(m_target, m_id, DEFAULT_TEXTURE_UNIT);
	glGenerateMipmap(m_target);
}

GLuint ej::Texture::getHandle() const
{
	return m_id;
}

GLenum ej::Texture::getTarget() const
{
	return m_target;
}

glm::uvec3 ej::Texture::getSize() const
{
	return m_size;
}
