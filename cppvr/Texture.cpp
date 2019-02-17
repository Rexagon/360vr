#include "Texture.h"

using namespace ej;

Texture::Texture(GLenum target) :
	m_target(target), m_internalFormat(GL_RGBA), m_format(GL_RGBA), m_type(GL_UNSIGNED_BYTE),
	m_minFilter(GL_LINEAR), m_maxFilter(GL_LINEAR),
	m_wrapS(GL_REPEAT), m_wrapT(GL_REPEAT), m_wrapR(GL_REPEAT),
	m_initialized(false), m_id(0)
{

}

Texture::~Texture()
{
	if (m_initialized) {
		glDeleteTextures(1, &m_id);
	}
}

bool Texture::init(unsigned int width, GLenum internalFormat, GLenum format, GLenum type, const void * data)
{
	if (m_initialized || m_target != GL_TEXTURE_1D || width == 0) {
		return false;
	}

	glGenTextures(1, &m_id);

	m_size = glm::ivec3(width, 0, 0);
	m_internalFormat = internalFormat;
	m_format = format;
	m_type = type;

	glBindTexture(m_target, m_id);

	glTexImage1D(m_target, 0, internalFormat, width, 0, format, type, data);
	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_minFilter);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_maxFilter);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_wrapS);
	glBindTexture(m_target, 0);

	m_initialized = true;
	return true;
}

bool Texture::init(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, const void * data)
{
	if (m_initialized || m_target != GL_TEXTURE_2D || width == 0 || height == 0) {
		return false;
	}

	glGenTextures(1, &m_id);

	m_size = glm::ivec3(width, height, 0);
	m_internalFormat = internalFormat;
	m_format = format;
	m_type = type;

	glBindTexture(m_target, m_id);

	glTexImage2D(m_target, 0, internalFormat, width, height, 0, format, type, data);
	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_minFilter);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_maxFilter);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_wrapS);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_wrapT);
	glBindTexture(m_target, 0);

	m_initialized = true;
	return true;
}

bool Texture::init(unsigned int width, unsigned int height, unsigned int depth, GLenum internalFormat, GLenum format, GLenum type, const void * data)
{
	if (m_initialized || m_target != GL_TEXTURE_3D || width == 0 || height == 0 || depth == 0) {
		return false;
	}

	glGenTextures(1, &m_id);

	m_size = glm::ivec3(width, height, depth);
	m_internalFormat = internalFormat;
	m_format = format;
	m_type = type;

	glBindTexture(m_target, m_id);
	glTexImage3D(m_target, 0, internalFormat, width, height, depth, 0, format, type, data);
	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_minFilter);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_maxFilter);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_wrapS);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_wrapT);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_R, m_wrapR);

	m_initialized = true;
	return true;
}

void Texture::resize(unsigned int width, unsigned int height, unsigned int depth)
{
	if (!m_initialized) return;

	glBindTexture(m_target, m_id);

	switch (m_target) {
	case GL_TEXTURE_1D:
		if (width == 0) return;
		m_size.x = width;
		glTexImage1D(GL_TEXTURE_1D, 0, m_internalFormat, width, 0, m_format, m_type, nullptr);
		break;

	case GL_TEXTURE_2D:
		if (width == 0 || height == 0) return;
		m_size.x = width;
		m_size.y = height;
		glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, width, height, 0, m_format, m_type, nullptr);
		break;

	case GL_TEXTURE_3D:
		if (width == 0 || height == 0 || depth == 0) return;
		m_size.x = width;
		m_size.y = height;
		m_size.z = height;
		glTexImage3D(GL_TEXTURE_3D, 0, m_internalFormat, width, height, depth, 0, m_format, m_type, nullptr);
		break;
	}
}

void Texture::setFilters(GLenum minFilter, GLenum maxFilter, bool bind)
{
	if (!m_initialized && bind || m_minFilter == minFilter && m_maxFilter == maxFilter) return;

	m_minFilter = minFilter;
	m_maxFilter = maxFilter;

	if (bind) {
		glBindTexture(m_target, m_id);

		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, maxFilter);
	}
}

void Texture::setWrapMode(GLenum wrapMode, bool bind)
{
	if (!m_initialized && bind) return;

	m_wrapR = wrapMode;
	m_wrapT = wrapMode;
	m_wrapS = wrapMode;

	if (bind) {
		glBindTexture(m_target, m_id);

		switch (m_target) {
		case GL_TEXTURE_3D:
			glTexParameteri(m_target, GL_TEXTURE_WRAP_R, wrapMode);

		case GL_TEXTURE_2D:
			glTexParameteri(m_target, GL_TEXTURE_WRAP_T, wrapMode);

		case GL_TEXTURE_1D:
			glTexParameteri(m_target, GL_TEXTURE_WRAP_S, wrapMode);
		}
	}
}

void Texture::generateMipmap() const
{
	if (!m_initialized) return;

	glBindTexture(m_target, m_id);
	glGenerateMipmap(m_target);
}

GLuint Texture::getHandle() const
{
	return m_id;
}

GLenum Texture::getTarget() const
{
	return m_target;
}

glm::ivec3 Texture::getSize() const
{
	return m_size;
}
