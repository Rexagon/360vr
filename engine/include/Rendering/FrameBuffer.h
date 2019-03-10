#pragma once

#include <GL/glew.h>

#include "Resources/Texture.h"

namespace ej 
{
	class RenderingManager;

	/**
	 * \brief OpenGL frame buffer wrapper
	 */
	class FrameBuffer final
	{
	public:
		/**
		 * \param core Main core object
		 */
		explicit FrameBuffer(const Core& core);

		/**
		 * \brief Destructor. Destroys all buffers
		 */
		~FrameBuffer();

		/**
		 * \brief Initialize frame buffer
		 * \param width frame buffer width
		 * \param height frame buffer height
		 * \param depthEnabled Depth buffer flag. If true depth buffer
		 * will be also created in current frame buffer
		 */
		void init(unsigned int width, unsigned int height, bool depthEnabled);

		/**
		 * \return true if initialized
		 */
		bool isInitialized() const;

		/**
		 * \brief Get color texture
		 * \return Texture
		 */
		Texture& getColorTexture();

		/**
		 * \return OpenGL native handle
		 */
		GLuint getHandle() const;

	private:
		RenderingManager* m_renderingManager = nullptr;

		bool m_isInitialized = false;

		GLuint m_id = 0;
		Texture m_colorTexture;
		GLuint m_depthBuffer = 0;
	};
}
