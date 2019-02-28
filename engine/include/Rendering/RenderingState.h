#pragma once

#include <SFML/Graphics/Color.hpp>

#include "Core/PointerDefs.h"
#include "Resources/Shader.h"
#include "Rendering/FrameBuffer.h"
#include "Managers/WindowManager.h"

namespace ej
{
	/**
	 * \brief Give access to rendering state with caching
	 * 
	 * All setters of this class wont make OpenGL call if it does not
	 * change it's state.
	 * 
	 * RenderState::apply() must be called before using state. Otherwise
	 * synchronization with real state is not guaranteed.
	 */
	class RenderingState final : public PointerDefs<RenderingState>
	{
	public:
		/**
		 * \brief Force apply current rendering state
		 */
		void apply();

		/**
		 * \brief Set current viewport parameters
		 * \param size Viewport size
		 * \param offset Viewport top left offset
		 */
		void setViewport(const glm::ivec2& size, const glm::ivec2& offset = glm::ivec2());

		/**
		 * \brief Set current viewport parameters
		 * \param x Viewport left offset
		 * \param y Viewport top offset
		 * \param width Viewport width
		 * \param height Viewport height
		 */
		void setViewport(GLint x, GLint y, GLsizei width, GLsizei height);

		/**
		 * \return Viewport size
		 */
		glm::ivec2 getViewportSize() const;

		/**
		 * \return viewport top left offset
		 */
		glm::ivec2 getViewportOffset() const;

		/**
		 * \param color RGBA, each component is in range [0, 1]
		 */
		void setClearColor(const glm::vec4& color);

		/**
		 * \param color Color
		 */
		void setClearColor(const sf::Color& color);

		/**
		 * \param r Red component in range [0, 1]
		 * \param g Green component in range [0, 1]
		 * \param b Blue component in range [0, 1]
		 * \param a Alpha component in range [0, 1]. 1.0 by default
		 */
		void setClearColor(float r, float g, float b, float a = 1.0f);

		/**
		 * Opaque black color is set by default.
		 * 
		 * \return Clear color
		 */
		glm::vec4 getClearColor() const;

		/**
		 * \brief Set clear depth value
		 * \param depth Depth value
		 */
		void setClearDepth(float depth);

		/**
		 * \brief Get clear depth value
		 * 
		 * Default depth value is 1
		 * 
		 * \return Depth value
		 */
		float getClearDepth() const;

		/**
		 * \brief Set depth test status
		 * \param enabled Status
		 */
		void setDepthTestEnabled(bool enabled);

		/**
		 * \brief Check if depth test is enabled
		 * 
		 * Depth test is enabled by default.
		 * 
		 * \return true if enabled
		 */
		bool isDepthTestEnabled() const;

		/**
		 * \brief Set depth write status
		 * \param enabled 
		 */
		void setDepthWriteEnabled(bool enabled);

		/**
		 * \brief Check if depth write is enabled
		 * 
		 * Depth write is enabled by default.
		 * 
		 * \return true if enabled
		 */
		bool isDepthWriteEnabled() const;

		/**
		 * \brief Set depth test function
		 * \param testFunction OpenGL depth test function.
		 * Must be one of GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL,
		 * GL_GREATER, GL_NOTEQUAL, GL_GEQUAL or GL_ALWAYS 
		 */
		void setDepthTestFunction(GLenum testFunction);

		/**
		 * \brief Get current depth test function
		 * 
		 * GL_LEQUAL is set by default.
		 * 
		 * \return Current depth test function
		 */
		GLenum getDepthTestFunction() const;

		/**
		 * \brief Control clip coordinate to window coordinate behavior
		 * \param origin Clip control origin. Must be one of
		 * GL_LOWER_LEFT or GL_UPPER_LEFT
		 * \param depth Clip control depth mode. Must be one of
		 * GL_NEGATIVE_ONE_TO_ONE or GL_ZERO_TO_ONE
		 */
		void setClipControl(GLenum origin, GLenum depth);

		/**
		 * \brief Get current clip control origin
		 * 
		 * GL_LOWER_LEFT is set by default.
		 * 
		 * \return Clip control origin
		 */
		GLenum getClipControlOrigin() const;

		/**
		 * \brief Get current clip control depth
		 * 
		 * GL_NEGATIVE_ONE_TO_ONE is set by default.
		 * 
		 * \return Clip control depth
		 */
		GLenum getClipControlDepth() const;

		/**
		 * \brief Set alpha blending status
		 * \param enabled Status
		 */
		void setBlendingEnabled(bool enabled);

		/**
		 * \brief Check if alpha blending is enabled
		 * 
		 * Disabled by default.
		 * 
		 * \return true if enabled
		 */
		bool isBlendingEnabled() const;

		/**
		 * \brief Specify pixel arithmetic
		 * 
		 * Functions must be one of: GL_ZERO, GL_ONE, GL_SRC_COLOR, 
		 * GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, 
		 * GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, 
		 * GL_ONE_MINUS_DST_ALPHA. GL_CONSTANT_COLOR, 
		 * GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, 
		 * and GL_ONE_MINUS_CONSTANT_ALPHA
		 * 
		 * \param src Source function
		 * \param dst Destination function
		 */
		void setBlendingFunctions(GLenum src, GLenum dst);

		/**
		 * \brief Get current alpha blending function for source
		 * 
		 * GL_SRC_ALPHA is set by default.
		 * 
		 * \return Blending function
		 */
		GLenum getBlendingFunctionSrc() const;

		/**
		 * \brief Get current alpha blending function for destination
		 * 
		 * GL_ONE_MINUS_SRC_ALPHA is set by default.
		 * 
		 * \return Blending function
		 */
		GLenum getBlendingFunctionDst() const;

		/**
		 * \brief Set face culling status
		 * \param enabled Status
		 */
		void setFaceCullingEnabled(bool enabled);

		/**
		 * \brief Check if face culling is enabled
		 * 
		 * Enabled by default.
		 * 
		 * \return true if enabled
		 */
		bool isFaceCullingEnabled() const;

		/**
		 * \brief Set face culling side
		 * \param side Culled side. Must be one of GL_FRONT, GL_BACK, 
		 * and GL_FRONT_AND_BACK
		 */
		void setFaceCullingSide(GLenum side);

		/**
		 * \brief Get face culling side
		 * 
		 * GL_BACK is set by default.
		 * 
		 * \return Side
		 */
		GLenum getFaceCullingSide() const;

		/**
		 * \brief Set polygon rendering mode
		 * \param mode Rendering mode. Must be one of GL_POINT, 
		 * GL_LINE, and GL_FILL
		 */
		void setPolygonMode(GLenum mode);

		/**
		 * \brief Get polygon rendering mode
		 * 
		 * GL_FILL is set by default.
		 * 
		 * \return Rendering mode
		 */
		GLenum getPolygonMode() const;

		/**
		 * \brief Select active texture unit
		 * \param unit Unit number. Can be any number, but only
		 * first 32 units can be cached by this manager.
		 */
		void setActiveTexture(unsigned int unit);

		/**
		 * \brief Bind current texture
		 * 
		 * For units in cached units range (0 to 31), \a texture value equal 
		 * to nullptr means that texture id '0' will be set. Texture target
		 * for these units wont change.
		 * 
		 * For other units texture target in case of nullptr \a texture value
		 * will be set to GL_TEXTURE_2D.
		 * 
		 * \param texture Current texture. Can be nullptr
		 * \param unit Unit number. Can be any number, but only
		 * first 32 units can be cached by this manager.
		 */
		void bindTexture(const Texture* texture, unsigned int unit);

		/**
		 * \brief Bind current texture
		 * \param textureTarget Target to which the texture is bound. Must be 
		 * one of GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, 
		 * GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_RECTANGLE, 
		 * GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BUFFER, 
		 * GL_TEXTURE_2D_MULTISAMPLE or GL_TEXTURE_2D_MULTISAMPLE_ARRAY
		 * \param textureId Texture id
		 * \param unit 
		 */
		void bindTexture(GLenum textureTarget, GLuint textureId, unsigned int unit);

		/**
		 * \brief Set current shader
		 * \param shader Shader. Can be nullptr
		 */
		void setCurrentShader(Shader* shader);

		/**
		 * \brief Set current shader
		 * \param shaderId Shader id
		 */
		void setCurrentShaderId(GLuint shaderId);

		/**
		 * \brief Get current shader
		 * 
		 * 0 is set by default.
		 * 
		 * \return Shader id
		 */
		GLuint getCurrentShaderId() const;

		/**
		 * \brief Set current frame buffer
		 * 
		 * Binds frame buffer as GL_FRAMEBUFFER.
		 * 
		 * \param frameBuffer Frame buffer. Can be nullptr
		 */
		void setCurrentFrameBuffer(FrameBuffer* frameBuffer);

		/**
		 * \brief Set current frame buffer
		 * 
		 * Binds frame buffer as GL_FRAMEBUFFER.
		 * 
		 * \param frameBufferId Frame buffer id
		 */
		void setCurrentFrameBufferId(GLuint frameBufferId);

		/**
		 * \brief Get current frame buffer
		 * \return Frame buffer id
		 */
		GLuint getCurrentFrameBufferId() const;

	private:
		static const size_t TEXTURE_COUNT = 32;

		GLint m_viewport[4]{};

		GLclampf m_clearColor[4]{ 0.0f, 0.0f, 0.0f, 1.0f };

		float m_clearDepth = 1.0f;

		bool m_isDepthTestEnabled = true;
		bool m_isDepthWriteEnabled = true;
		GLenum m_depthTestFunction = GL_LEQUAL;

		GLenum m_clipControlOrigin = GL_LOWER_LEFT;
		GLenum m_clipControlDepth = GL_NEGATIVE_ONE_TO_ONE;

		bool m_isBlendingEnabled = false;
		GLenum m_blendingFunctionSrc = GL_SRC_ALPHA;
		GLenum m_blendingFunctionDst = GL_ONE_MINUS_SRC_ALPHA;

		bool m_isFaceCullingEnabled = true;
		GLenum m_faceCullingSide = GL_BACK;

		GLenum m_polygonMode = GL_FILL;

		size_t m_activeTextureUnit = 0;

		GLuint m_currentShaderId = 0;
		GLuint m_currentFrameBufferId = 0;
		std::pair<GLenum, GLuint> m_currentTextures[TEXTURE_COUNT]{std::pair(0, 0)};
	};
}
