#pragma once

#include <GL/glew.h>

#include "MeshGeometry.h"
#include "Core/PointerDefs.h"

namespace ej
{
	/**
	 * \brief Drawable object
	 */
	class Mesh final : public PointerDefs<Mesh>
	{
	public:
		/**
		 * \brief Destructor. Destroys all buffers
		 */
		~Mesh();

		/**
		 * \brief Initialize buffers and set mesh content
		 * 
		 * Only first call will affect this object. For
		 * updating geometry use Mesh::update().
		 * 
		 * \param geometry Mesh content
		 */
		void init(const MeshGeometry& geometry);

		/**
		 * \brief Update buffers with new geometry
		 * \param geometry Mesh content
		 */
		void update(const MeshGeometry& geometry);

		/**
		 * \brief Draw mesh
		 */
		void draw() const;

		/**
		 * \return Index count
		 */
		unsigned int getIndexCount() const;

		/**
		 * \return Vertex count
		 */
		unsigned int getVertexCount() const;

		/**
		 * \brief Get number of mesh vertex components
		 * \return Attribute count
		 */
		unsigned int getAttributeCount() const;

		/**
		 * \brief Set buffer usage mode
		 * \param usage Usage mode. Must be one of GL_STREAM_DRAW, 
		 * GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, 
		 * GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY
		 */
		void setBufferUsage(GLenum usage);

		/**
		 * \brief Get buffer usage mode
		 * 
		 * GL_STATIC_DRAW is set by default.
		 * 
		 * \return Usage mode
		 */
		GLenum getBufferUsage() const;

		/**
		 * \return true if initialized
		 */
		bool isInitialized() const;

	private:
		GLuint m_vao = 0;
		GLuint m_vbo = 0;
		GLuint m_ebo = 0;

		unsigned int m_indexCount = 0;
		unsigned int m_vertexCount = 0;
		unsigned int m_attributeCount = 0;

		GLenum m_topology = 0;

		GLenum m_bufferUsage = GL_STATIC_DRAW;
		bool m_isInitialized = false;
	};
}
