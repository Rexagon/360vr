#include "Mesh.h"

using namespace ej;

Mesh::Mesh() :
	m_isInitialized(false)
{
	glGenVertexArrays(1, &m_VAO);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void Mesh::init(const MeshGeometry& geometry)
{
	if (m_isInitialized) return;

	// Calculating buffer layout
	m_topology = geometry.topology;

	m_indexCount = static_cast<unsigned int>(geometry.indices.size());
	m_vertexCount = 0;
	m_attributeCount = 0;
	m_vertexCount = static_cast<unsigned int>(geometry.positions.size());

	size_t bufferSize = 0;

	size_t positionsBufferSize = 0;
	if (geometry.vertexComponents & MeshGeometry::POSITIONS) {
		positionsBufferSize = sizeof(glm::vec3) * m_vertexCount;
		bufferSize += positionsBufferSize;
		++m_attributeCount;
	}

	size_t texCoordsBufferSize = 0;
	if (geometry.vertexComponents & MeshGeometry::TEX_COORDS) {
		texCoordsBufferSize = sizeof(glm::vec3) * m_vertexCount;
		bufferSize += texCoordsBufferSize;
		++m_attributeCount;
	}

	size_t normalsBufferSize = 0;
	if (geometry.vertexComponents & MeshGeometry::NORMALS) {
		normalsBufferSize = sizeof(glm::vec3) * m_vertexCount;
		bufferSize += normalsBufferSize;
		++m_attributeCount;
	}

	size_t tangetnsBufferSize = 0;
	if (geometry.vertexComponents & MeshGeometry::TANGENTS) {
		tangetnsBufferSize = sizeof(glm::vec3) * m_vertexCount;
		bufferSize += tangetnsBufferSize;
		++m_attributeCount;
	}

	size_t bitangentsBufferSize = 0;
	if (geometry.vertexComponents & MeshGeometry::BITANGENTS) {
		bitangentsBufferSize = sizeof(glm::vec3) * m_vertexCount;
		bufferSize += bitangentsBufferSize;
		++m_attributeCount;
	}

	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	std::vector<char> data(bufferSize);

	size_t offset = 0;
	if (geometry.vertexComponents & MeshGeometry::POSITIONS) {
		std::memcpy(&data[0] + offset, geometry.positions.data(), positionsBufferSize);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(offset));

		offset += positionsBufferSize;
	}

	if (geometry.vertexComponents & MeshGeometry::TEX_COORDS) {
		std::memcpy(&data[0] + offset, geometry.texCoords.data(), texCoordsBufferSize);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(offset));

		offset += texCoordsBufferSize;
	}

	if (geometry.vertexComponents & MeshGeometry::NORMALS) {
		std::memcpy(&data[0] + offset, geometry.normals.data(), normalsBufferSize);
		glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(offset));

		offset += normalsBufferSize;
	}

	glBufferData(GL_ARRAY_BUFFER, bufferSize, data.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indexCount, &geometry.indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	m_isInitialized = true;
}

void Mesh::draw() const
{
	glBindVertexArray(m_VAO);
	for (unsigned int i = 0; i < m_attributeCount; ++i) {
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glDrawElements(m_topology, m_indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	for (unsigned int i = 0; i < m_attributeCount; ++i) {
		glDisableVertexAttribArray(i);
	}
	glBindVertexArray(0);
}

unsigned int Mesh::getIndexCount() const
{
	return m_indexCount;
}

unsigned int Mesh::getVertexCount() const
{
	return m_vertexCount;
}

unsigned int Mesh::getAttributeCount() const
{
	return m_attributeCount;
}

bool Mesh::isInitialized() const
{
	return m_isInitialized;
}
