#include "MeshGeometry.h"

#include <glm/gtc/constants.hpp>

using namespace ej;

MeshGeometry MeshGeometry::createQuad(const glm::vec2 & halfSize, ComponentsMask vertexComponents)
{
	MeshGeometry result(vertexComponents);

	if (vertexComponents & POSITIONS)
		result.positions = {
		glm::vec3(-halfSize.x,  halfSize.y, 0.0f),
		glm::vec3(halfSize.x,  halfSize.y, 0.0f),
		glm::vec3(halfSize.x, -halfSize.y, 0.0f),
		glm::vec3(-halfSize.x, -halfSize.y, 0.0f)
	};

	if (vertexComponents & TEX_COORDS) {
		result.texCoords = {
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		};
	}

	if (vertexComponents & NORMALS) {
		result.normals = std::vector<glm::vec3>(4, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	result.indices = {
		0, 2, 1,
		0, 3, 2
	};

	return result;
}

MeshGeometry MeshGeometry::createPlane(const glm::vec2 & halfSize, unsigned int xSegments, unsigned int ySegments,
	ComponentsMask vertexComponents)
{
	MeshGeometry result(vertexComponents, GL_TRIANGLE_STRIP);

	float dX = 1.0f / xSegments;
	float dY = 1.0f / ySegments;

	for (int y = 0; y <= static_cast<int>(ySegments); ++y)
	{
		for (int x = 0; x <= static_cast<int>(xSegments); ++x)
		{
			if (vertexComponents & POSITIONS) {
				result.positions.push_back(glm::vec3(dX * x * 2.0f - 1.0f, 0.0f, dY * y * 2.0f - 1.0f));
			}

			if (vertexComponents & TEX_COORDS) {
				result.texCoords.push_back(glm::vec2(dX * x, 1.0f - y * dY));
			}

			if (vertexComponents & NORMALS) {
				result.normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			}
		}
	}

	bool oddRow = false;
	for (int y = 0; y < static_cast<int>(ySegments); ++y)
	{
		if (!oddRow)
		{
			for (int x = 0; x <= static_cast<int>(xSegments); ++x)
			{
				result.indices.push_back(y       * (xSegments + 1) + x);
				result.indices.push_back((y + 1) * (xSegments + 1) + x);
			}
		}
		else
		{
			for (int x = static_cast<int>(xSegments); x >= 0; --x)
			{
				result.indices.push_back((y + 1) * (xSegments + 1) + x);
				result.indices.push_back(y       * (xSegments + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	return result;
}

MeshGeometry MeshGeometry::createCube(const glm::vec3 & halfSize, ComponentsMask vertexComponents)
{
	MeshGeometry result(vertexComponents);

	if (vertexComponents & POSITIONS) {
		result.positions = {
			// TOP
			glm::vec3(-halfSize.x,  halfSize.y, -halfSize.z),
			glm::vec3(halfSize.x,  halfSize.y, -halfSize.z),
			glm::vec3(halfSize.x,  halfSize.y,  halfSize.z),
			glm::vec3(-halfSize.x,  halfSize.y,  halfSize.z),

			// DOWN
			glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z),
			glm::vec3(halfSize.x, -halfSize.y, -halfSize.z),
			glm::vec3(halfSize.x, -halfSize.y,  halfSize.z),
			glm::vec3(-halfSize.x, -halfSize.y,  halfSize.z),

			// FRONT
			glm::vec3(-halfSize.x,  halfSize.y, halfSize.z),
			glm::vec3(halfSize.x,  halfSize.y, halfSize.z),
			glm::vec3(halfSize.x, -halfSize.y, halfSize.z),
			glm::vec3(-halfSize.x, -halfSize.y, halfSize.z),

			// BACK
			glm::vec3(-halfSize.x,  halfSize.y, -halfSize.z),
			glm::vec3(halfSize.x,  halfSize.y, -halfSize.z),
			glm::vec3(halfSize.x, -halfSize.y, -halfSize.z),
			glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z),

			// RIGHT
			glm::vec3(halfSize.x,  halfSize.y, -halfSize.z),
			glm::vec3(halfSize.x,  halfSize.y,  halfSize.z),
			glm::vec3(halfSize.x, -halfSize.y,  halfSize.z),
			glm::vec3(halfSize.x, -halfSize.y, -halfSize.z),

			// LEFT
			glm::vec3(-halfSize.x,  halfSize.y, -halfSize.z),
			glm::vec3(-halfSize.x,  halfSize.y,  halfSize.z),
			glm::vec3(-halfSize.x, -halfSize.y,  halfSize.z),
			glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z),
		};
	}

	if (vertexComponents & TEX_COORDS) {
		result.texCoords = {
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),
		};
	}

	if (vertexComponents & NORMALS) {
		result.normals = {
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),

			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),

			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),

			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
		};
	}

	result.indices = {
		0, 2, 1,
		0, 3, 2,

		4, 5, 6,
		4, 6, 7,

		8, 10, 9,
		8, 11, 10,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 22, 21,
		20, 23, 22
	};

	return result;
}

MeshGeometry MeshGeometry::createSphere(float radius, unsigned int xSegments, unsigned int ySegments,
	ComponentsMask vertexComponents)
{
	MeshGeometry result(vertexComponents);

	for (unsigned int y = 0; y <= ySegments; ++y)
	{
		for (unsigned int x = 0; x <= xSegments; ++x)
		{
			float xSegment = (float)x / (float)ySegments;
			float ySegment = (float)y / (float)ySegments;
			float xPos = std::cos(xSegment * glm::two_pi<float>()) * std::sin(ySegment * glm::pi<float>());
			float yPos = std::cos(ySegment * glm::pi<float>());
			float zPos = std::sin(xSegment * glm::two_pi<float>()) * std::sin(ySegment * glm::pi<float>());

			if (vertexComponents & POSITIONS) {
				result.positions.push_back(glm::vec3(xPos, yPos, zPos));
			}

			if (vertexComponents & TEX_COORDS) {
				result.texCoords.push_back(glm::vec2(xSegment, ySegment));
			}

			if (vertexComponents & NORMALS) {
				result.normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}
	}


	bool oddRow = false;
	for (unsigned int y = 0; y < ySegments; ++y)
	{
		for (unsigned int x = 0; x < xSegments; ++x)
		{
			result.indices.push_back((y + 1) * (xSegments + 1) + x);
			result.indices.push_back(y       * (xSegments + 1) + x);
			result.indices.push_back(y       * (xSegments + 1) + x + 1);

			result.indices.push_back((y + 1) * (xSegments + 1) + x);
			result.indices.push_back(y       * (xSegments + 1) + x + 1);
			result.indices.push_back((y + 1) * (xSegments + 1) + x + 1);
		}
	}

	return result;
}

MeshGeometry::MeshGeometry(ComponentsMask vertexComponents, GLenum topology) :
	vertexComponents(vertexComponents), topology(topology)
{
}

void ej::MeshGeometry::prepareArrays(unsigned int indexCount)
{
	if (vertexComponents & POSITIONS) {
		positions.resize(indexCount);
	}
	if (vertexComponents & TEX_COORDS) {
		texCoords.resize(indexCount);
	}
	if (vertexComponents & TEX_COORDS) {
		normals.resize(indexCount);
	}
	if (vertexComponents & TANGENTS) {
		tangents.resize(indexCount);
	}
	if (vertexComponents & BITANGENTS) {
		bitangents.resize(indexCount);
	}
}
