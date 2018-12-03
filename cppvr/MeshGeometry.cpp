#include "MeshGeometry.h"

#include <glm/gtc/constants.hpp>

using namespace ej;

MeshGeometry MeshGeometry::createQuad(const glm::vec2 & halfSize, ComponentsMask vertexComponents)
{
	MeshGeometry result(vertexComponents);

	if (vertexComponents & Positions)
		result.positions = {
		glm::vec3(-halfSize.x,  halfSize.y, 0.0f),
		glm::vec3(halfSize.x,  halfSize.y, 0.0f),
		glm::vec3(halfSize.x, -halfSize.y, 0.0f),
		glm::vec3(-halfSize.x, -halfSize.y, 0.0f)
	};

	if (vertexComponents & TexCoords) {
		result.texCoords = {
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		};
	}

	if (vertexComponents & Normals) {
		result.normals = std::vector<glm::vec3>(4, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	result.indices = {
		0, 2, 1,
		0, 3, 2
	};

	return result;
}

MeshGeometry MeshGeometry::createPlane(const glm::vec2 & halfSize, const unsigned int xSegments, const unsigned int ySegments,
                                       const ComponentsMask vertexComponents)
{
	MeshGeometry result(vertexComponents, GL_TRIANGLE_STRIP);

	const auto dX = 1.0f / xSegments;
	const auto dY = 1.0f / ySegments;

	for (auto y = 0; y <= static_cast<int>(ySegments); ++y) {
		for (auto x = 0; x <= static_cast<int>(xSegments); ++x) {
			if (vertexComponents & Positions) {
				result.positions.emplace_back(halfSize.x * (dX * x * 2.0f - 1.0f), 0.0f, halfSize.y * (dY * y * 2.0f - 1.0f));
			}

			if (vertexComponents & TexCoords) {
				result.texCoords.emplace_back(dX * x, 1.0f - y * dY);
			}

			if (vertexComponents & Normals) {
				result.normals.emplace_back(0.0f, 1.0f, 0.0f);
			}
		}
	}

	auto oddRow = false;
	for (auto y = 0; y < static_cast<int>(ySegments); ++y) {
		if (!oddRow) {
			for (auto x = 0; x <= static_cast<int>(xSegments); ++x) {
				result.indices.push_back(y       * (xSegments + 1) + x);
				result.indices.push_back((y + 1) * (xSegments + 1) + x);
			}
		}
		else {
			for (auto x = static_cast<int>(xSegments); x >= 0; --x) {
				result.indices.push_back((y + 1) * (xSegments + 1) + x);
				result.indices.push_back(y       * (xSegments + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	return result;
}

MeshGeometry MeshGeometry::createCube(const glm::vec3 & halfSize, const ComponentsMask vertexComponents)
{
	MeshGeometry result(vertexComponents);

	if (vertexComponents & Positions) {
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

	if (vertexComponents & TexCoords) {
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

	if (vertexComponents & Normals) {
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

MeshGeometry MeshGeometry::createSphere(float radius, const unsigned int xSegments, const unsigned int ySegments,
                                        const ComponentsMask vertexComponents)
{
	MeshGeometry result(vertexComponents);

	for (unsigned int y = 0; y <= ySegments; ++y) {
		for (unsigned int x = 0; x <= xSegments; ++x) {
			const auto xSegment = static_cast<float>(x) / static_cast<float>(ySegments);
			const auto ySegment = static_cast<float>(y) / static_cast<float>(ySegments);
			const auto xPos = std::cos(xSegment * glm::two_pi<float>()) * std::sin(ySegment * glm::pi<float>());
			const auto yPos = std::cos(ySegment * glm::pi<float>());
			const auto zPos = std::sin(xSegment * glm::two_pi<float>()) * std::sin(ySegment * glm::pi<float>());

			if (vertexComponents & Positions) {
				result.positions.emplace_back(xPos, yPos, zPos);
			}

			if (vertexComponents & TexCoords) {
				result.texCoords.emplace_back(xSegment, ySegment);
			}

			if (vertexComponents & Normals) {
				result.normals.emplace_back(xPos, yPos, zPos);
			}
		}
	}

	auto oddRow = false;
	for (unsigned int y = 0; y < ySegments; ++y) {
		for (unsigned int x = 0; x < xSegments; ++x) {
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

MeshGeometry::MeshGeometry(const ComponentsMask vertexComponents, const GLenum topology) :
	vertexComponents(vertexComponents), topology(topology)
{
}

void ej::MeshGeometry::prepareArrays(const unsigned int indexCount)
{
	if (vertexComponents & Positions) {
		positions.resize(indexCount);
	}
	if (vertexComponents & TexCoords) {
		texCoords.resize(indexCount);
	}
	if (vertexComponents & TexCoords) {
		normals.resize(indexCount);
	}
	if (vertexComponents & Tangents) {
		tangents.resize(indexCount);
	}
	if (vertexComponents & Bitangents) {
		bitangents.resize(indexCount);
	}
}
