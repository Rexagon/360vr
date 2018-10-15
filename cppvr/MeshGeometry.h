#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace ej
{
	struct MeshGeometry
	{
	public:
		typedef char ComponentsMask;

		enum Components {
			POSITIONS = 1 << 0,
			TEX_COORDS = 1 << 1,
			NORMALS = 1 << 2,
			TANGENTS = 1 << 3,
			BITANGENTS = 1 << 4,
		};

		static const ComponentsMask SIMPLE_VERTEX = POSITIONS;
		static const ComponentsMask TEXTURED_VERTEX = POSITIONS | TEX_COORDS;
		static const ComponentsMask MODEL_VERTEX = POSITIONS | TEX_COORDS | NORMALS | TANGENTS | BITANGENTS;

		static MeshGeometry createQuad(const glm::vec2& halfSize = glm::vec2(1.0f),
			ComponentsMask vertexComponents = MODEL_VERTEX);

		static MeshGeometry createPlane(const glm::vec2& halfSize = glm::vec2(5.0f), unsigned int xSegments = 1, unsigned int ySegments = 1,
			ComponentsMask vertexComponents = MODEL_VERTEX);

		static MeshGeometry createCube(const glm::vec3& halfSize = glm::vec3(1.0f),
			ComponentsMask vertexComponents = MODEL_VERTEX);

		static MeshGeometry createSphere(float radius, unsigned int xSegments, unsigned int ySegments,
			ComponentsMask vertexComponents = MODEL_VERTEX);

		MeshGeometry(ComponentsMask vertexComponents = MODEL_VERTEX, GLenum topology = GL_TRIANGLES);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;

		std::vector<unsigned int> indices;

		ComponentsMask vertexComponents;
		GLenum topology;

	private:
		void prepareArrays(unsigned int indexCount);
	};
}