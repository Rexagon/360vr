#pragma once

#include <memory>

#include <GL/glew.h>
#include <glm/vec3.hpp>

#include "Core/Core.h"
#include "Core/PointerDefs.h"

namespace ej 
{
	class RenderingState;

	/**
	 * \brief Image living on the graphics card that can be used for drawing
	 */
	class Texture final : public PointerDefs<Texture>
	{
	public:
		/**
		 * \param core Main core object
		 * \param target Target to which the texture is bound. Must be 
		 * one of GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, 
		 * GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_RECTANGLE, 
		 * GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BUFFER, 
		 * GL_TEXTURE_2D_MULTISAMPLE or GL_TEXTURE_2D_MULTISAMPLE_ARRAY
		 */
		explicit Texture(const Core& core, GLenum target = GL_TEXTURE_2D);
		~Texture();

		/**
		 * \brief Initialize as 1D texture
		 * 
		 * Texture can be initialized only once.
		 * 
		 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage1D.xhtml
		 * 
		 * \param width Texture width in pixels
		 * \param internalFormat number of color components in the texture
		 * \param format format of the pixel data. Must be one of GL_RED, 
		 * GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_RED_INTEGER, 
		 * GL_RG_INTEGER, GL_RGB_INTEGER, GL_BGR_INTEGER, GL_RGBA_INTEGER, 
		 * GL_BGRA_INTEGER, GL_STENCIL_INDEX, GL_DEPTH_COMPONENT or
		 * GL_DEPTH_STENCIL
		 * \param type Data type of the pixel data. Must be one of 
		 * GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, 
		 * GL_UNSIGNED_INT, GL_INT, GL_HALF_FLOAT, GL_FLOAT and etc.
		 * \param data Pointer to the image data in memory
		 * \return true if successfully initialized
		 */
		bool init(unsigned int width,
			GLenum internalFormat, GLenum format, GLenum type, const void* data);

		/**
		 * \brief Initialize as 2D texture
		 *
		 * Texture can be initialized only once.
		 *
		 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
		 *
		 * \param width Texture width in pixels
		 * \param height Texture height in pixels
		 * \param internalFormat number of color components in the texture
		 * \param format format of the pixel data. Must be one of GL_RED,
		 * GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_RED_INTEGER,
		 * GL_RG_INTEGER, GL_RGB_INTEGER, GL_BGR_INTEGER, GL_RGBA_INTEGER,
		 * GL_BGRA_INTEGER, GL_STENCIL_INDEX, GL_DEPTH_COMPONENT or
		 * GL_DEPTH_STENCIL
		 * \param type Data type of the pixel data. Must be one of
		 * GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT,
		 * GL_UNSIGNED_INT, GL_INT, GL_HALF_FLOAT, GL_FLOAT and etc.
		 * \param data Pointer to the image data in memory
		 * \return true if successfully initialized
		 */
		bool init(unsigned int width, unsigned int height,
			GLenum internalFormat, GLenum format, GLenum type, const void* data);

		/**
		 * \brief Initialize as 3D texture
		 *
		 * Texture can be initialized only once.
		 *
		 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage3D.xhtml
		 *
		 * \param width Texture width in pixels. Must be > 1
		 * \param height Texture height in pixels. Must be > 1
		 * \param depth Texture depth in pixels. Must be > 1
		 * \param internalFormat number of color components in the texture
		 * \param format format of the pixel data. Must be one of GL_RED,
		 * GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_RED_INTEGER,
		 * GL_RG_INTEGER, GL_RGB_INTEGER, GL_BGR_INTEGER, GL_RGBA_INTEGER,
		 * GL_BGRA_INTEGER, GL_STENCIL_INDEX, GL_DEPTH_COMPONENT or
		 * GL_DEPTH_STENCIL
		 * \param type Data type of the pixel data. Must be one of
		 * GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT,
		 * GL_UNSIGNED_INT, GL_INT, GL_HALF_FLOAT, GL_FLOAT and etc.
		 * \param data Pointer to the image data in memory
		 * \return true if successfully initialized
		 */
		bool init(unsigned int width, unsigned int height, unsigned int depth,
			GLenum internalFormat, GLenum format, GLenum type, const void* data);

		/**
		 * \brief Change size of texture
		 * 
		 * Only needed components will be used.
		 * E.g. for TEXTURE_2D - width and height.
		 * 
		 * \param width New texture with in pixels
		 * \param height New texture height in pixels
		 * \param depth New texture depth in pixels
		 */
		void resize(unsigned int width, unsigned int height = 0, unsigned int depth = 0);

		/**
		 * \brief Change sampling arithmetic
		 * 
		 * When this function is called before initialization, it just set
		 * parameters locally. When it is called after initialization it will
		 * change OpenGL state of this texture.
		 * 
		 * Filter parameters must be one of: GL_NEAREST, GL_LINEAR
		 * 
		 * \param minFilter Filter used for texture minimization
		 * \param maxFilter Filter used for texture magnification
		 */
		void setFilters(GLenum minFilter, GLenum maxFilter);

		/**
		 * \brief Change texture wrapping for all dimensions
		 * 
		 * When this function is called before initialization, it just set
		 * parameters locally. When it is called after initialization it will
		 * change OpenGL state of this texture.
		 * 
		 * \param wrapMode Component values arithmetic outside [0, 1]. Must 
		 * be one of: GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT,
		 * GL_REPEAT, or GL_MIRROR_CLAMP_TO_EDGE
		 */
		void setWrapMode(GLenum wrapMode);

		/**
		 * \brief Change texture wrapping for all dimensions
		 *
		 * When this function is called before initialization, it just set
		 * parameters locally. When it is called after initialization it will
		 * change OpenGL state of this texture.
		 *
		 * \param wrapMode Component values arithmetic outside [0, 1]. Must
		 * be one of: GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT,
		 * GL_REPEAT, or GL_MIRROR_CLAMP_TO_EDGE
		 * \param component Component number. 0 - width, 1 - height, 2 - depth
		 */
		void setWrapMode(GLenum wrapMode, size_t component);

		/**
		 * \brief Generates LOD textures
		 * 
		 * Must be called only after initialization
		 */
		void generateMipmap() const;

		/**
		 * \return Native OpenGL handle
		 */
		GLuint getHandle() const;

		/**
		 * \return Target to which the texture is bound
		 */
		GLenum getTarget() const;

		/**
		 * \return Texture dimensions. Useless components will be zeroed
		 */
		glm::uvec3 getSize() const;

	private:
		static const unsigned DEFAULT_TEXTURE_UNIT = 16;

		RenderingState* m_renderingState = nullptr;

		GLuint m_id = 0;
		GLenum m_target;

		GLenum m_internalFormat = GL_RGBA;
		GLenum m_format = GL_RGBA;
		GLenum m_type = GL_UNSIGNED_BYTE;

		GLenum m_minFilter = GL_LINEAR;
		GLenum m_maxFilter = GL_LINEAR;

		GLenum m_wrapS = GL_REPEAT;
		GLenum m_wrapT = GL_REPEAT;
		GLenum m_wrapR = GL_REPEAT;

		glm::uvec3 m_size{};

		bool m_initialized = false;
	};
}
