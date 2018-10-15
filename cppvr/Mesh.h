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
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;

		unsigned int m_indexCount;
		unsigned int m_vertexCount;
		unsigned int m_attributeCount;

		GLenum m_topology;

		bool m_isInitialized;
	};
}