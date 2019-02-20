#include "Resources/Mesh.h"

using namespace ej;

Mesh::Mesh()
{
	glGenVertexArrays(1, &m_vao);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
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
	if (geometry.vertexComponents & MeshGeometry::Positions) {
		positionsBufferSize = sizeof(glm::vec3) * geometry.positions.size();
		bufferSize += positionsBufferSize;
		++m_attributeCount;
	}

	size_t texCoordsBufferSize = 0;
	if (geometry.vertexComponents & MeshGeometry::TexCoords) {
		texCoordsBufferSize = sizeof(glm::vec2) * geometry.texCoords.size();
		bufferSize += texCoordsBufferSize;
		++m_attributeCount;
	}

	size_t normalsBufferSize = 0;
	if (geometry.vertexComponents & MeshGeometry::Normals) {
		normalsBufferSize = sizeof(glm::vec3) * geometry.normals.size();
		bufferSize += normalsBufferSize;
		++m_attributeCount;
	}

	size_t tangentsBufferSize = 0;
	if (geometry.vertexComponents & MeshGeometry::Tangents) {
		tangentsBufferSize = sizeof(glm::vec3) * geometry.tangents.size();
		bufferSize += tangentsBufferSize;
		++m_attributeCount;
	}

	size_t bitangentsBufferSize = 0;
	if (geometry.vertexComponents & MeshGeometry::Bitangents) {
		bitangentsBufferSize = sizeof(glm::vec3) * geometry.bitangents.size();
		bufferSize += bitangentsBufferSize;
		++m_attributeCount;
	}

	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	std::vector<char> data(bufferSize);

	size_t offset = 0;
	if (geometry.vertexComponents & MeshGeometry::Positions) {
		std::memcpy(&data[0] + offset, geometry.positions.data(), positionsBufferSize);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(offset));

		offset += positionsBufferSize;
	}

	if (geometry.vertexComponents & MeshGeometry::TexCoords) {
		std::memcpy(&data[0] + offset, geometry.texCoords.data(), texCoordsBufferSize);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(offset));

		offset += texCoordsBufferSize;
	}

	if (geometry.vertexComponents & MeshGeometry::Normals) {
		std::memcpy(&data[0] + offset, geometry.normals.data(), normalsBufferSize);
		glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(offset));

		offset += normalsBufferSize;
	}

	if (geometry.vertexComponents & MeshGeometry::Tangents) {
		std::memcpy(&data[0] + offset, geometry.tangents.data(), tangentsBufferSize);
		glVertexAttribPointer(3, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(offset));

		offset += tangentsBufferSize;
	}

	if (geometry.vertexComponents & MeshGeometry::Bitangents) {
		std::memcpy(&data[0] + offset, geometry.bitangents.data(), bitangentsBufferSize);
		glVertexAttribPointer(4, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(offset));

		offset += bitangentsBufferSize;
	}
	
	glBufferData(GL_ARRAY_BUFFER, bufferSize, data.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indexCount, &geometry.indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	m_isInitialized = true;
}

void Mesh::draw() const
{
	glBindVertexArray(m_vao);
	for (unsigned int i = 0; i < m_attributeCount; ++i) {
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
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
