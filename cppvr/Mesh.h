#pragma once

#include <GL/glew.h>

#include "MeshGeometry.h"

namespace ej
{
	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		void init(const MeshGeometry& geometry);
		void draw() const;

		unsigned int getIndexCount() const;
		unsigned int getVertexCount() const;
		unsigned int getAttributeCount() const;

		bool isInitialized() const;

	private:
		GLuint m_vao = 0;
		GLuint m_vbo = 0;
		GLuint m_ebo = 0;

		unsigned int m_indexCount = 0;
		unsigned int m_vertexCount = 0;
		unsigned int m_attributeCount = 0;

		GLenum m_topology = 0;

		bool m_isInitialized = false;
	};
}