#include "Managers/MeshManager.h"

#include "Managers/FileManager.h"

namespace details
{
	struct Vertex final
	{
		size_t positionIndex;
		size_t texCoordsIndex;
		size_t normalIndex;
	};
}

static std::vector<std::string_view> split(std::string_view string, const char delimiter)
{
	if (string.empty()) {
		return {};
	}

	std::vector<std::string_view> result;

	auto begin = std::begin(string);
	const auto end = std::end(string);

	std::string_view::iterator next;
	do {
		next = std::find(begin, end, delimiter);
		result.emplace_back(&*begin, next - begin);
		begin = next + 1;
	} while (next != end);

	return result;
}

ej::MeshManager::MeshManager(const Core& core) :
	ResourceManager(core)
{
	m_fileManager = core.get<FileManager>();
}

std::unique_ptr<ej::Mesh> ej::MeshManager::load(const std::string& filename)
{
	auto data = m_fileManager->open(filename);

	MeshGeometry meshGeometry;
	meshGeometry.vertexComponents = 0;
	meshGeometry.topology = GL_TRIANGLES;

	std::vector<details::Vertex> vertices;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> normals;

	std::unordered_map<std::string_view, unsigned int> verticesToIndices;

	size_t origin = 0;
	for (size_t current = 0; current < data.size(); ++current) {
		if (data[current] != '\n' && current + 1 != data.size()) {
			continue;
		}

		const auto tokens = split(std::string_view{ data.c_str() + origin, current - origin }, ' ');
		origin = current + 1;

		if (tokens.size() < 2) {
			continue;
		}

		if (tokens[0] == "v" && tokens.size() == 4) {
			// Parse position
			glm::vec3 position;
			for (size_t i = 0; i < 3; ++i) {
				// Waiting for C++20 with non-owning stoXX functions....
				position[i] = std::stof(std::string{ tokens[i + 1] });
			}
			positions.emplace_back(position);
		}
		else if (tokens[0] == "vt" && tokens.size() == 3) {
			// Parse texCoords
			glm::vec2 texCoords;
			for (size_t i = 0; i < 2; ++i) {
				texCoords[i] = std::stof(std::string{ tokens[i + 1] });
			}
			texCoords.y = 1.0f - texCoords.y;
			textureCoords.emplace_back(texCoords);
		}
		else if (tokens[0] == "vn" && tokens.size() == 4) {
			// Parse normal
			glm::vec3 normal;
			for (size_t i = 0; i < 2; ++i) {
				normal[i] = std::stof(std::string{ tokens[i + 1] });
			}
			normals.emplace_back(normal);
		}
		else if (tokens[0] == "f" && tokens.size() == 4) {
			// Parse face
			for (size_t i = 0; i < 3; ++i) {
				const auto& token = tokens[i + 1];
				auto it = verticesToIndices.find(token);

				if (it != verticesToIndices.end()) {
					meshGeometry.indices.emplace_back(it->second);
					continue;
				}

				const auto index = static_cast<unsigned int>(vertices.size());

				vertices.emplace_back();
				auto& vertex = vertices.back();

				const auto subtokens = split(token, '/');
				if (subtokens.size() > 0) {
					vertex.positionIndex = std::stoull(std::string{ subtokens[0] }) - 1;
				}
				if (subtokens.size() > 1) {
					vertex.texCoordsIndex = std::stoull(std::string{ subtokens[1] }) - 1;
				}
				if (subtokens.size() > 2) {
					vertex.normalIndex = std::stoull(std::string{ subtokens[2] }) - 1;
				}

				verticesToIndices.emplace(token, index);
				meshGeometry.indices.emplace_back(index);
			}
		}
	}

	data.clear();

	const auto hasPositions = !positions.empty();
	const auto hasTexCoords = !textureCoords.empty();
	const auto hasNormals = !normals.empty();

	if (hasPositions) {
		meshGeometry.vertexComponents |= MeshGeometry::Positions;
		meshGeometry.positions.reserve(positions.size());
	}
	if (hasTexCoords) {
		meshGeometry.vertexComponents |= MeshGeometry::TexCoords;
		meshGeometry.texCoords.reserve(textureCoords.size());
	}
	if (hasNormals) {
		meshGeometry.vertexComponents |= MeshGeometry::Normals;
		meshGeometry.normals.reserve(normals.size());
	}

	for (const auto& vertex : vertices) {
		if (hasPositions) {
			meshGeometry.positions.emplace_back(positions[vertex.positionIndex]);
		}
		if (hasTexCoords) {
			meshGeometry.texCoords.emplace_back(positions[vertex.texCoordsIndex]);
		}
		if (hasNormals) {
			meshGeometry.normals.emplace_back(positions[vertex.normalIndex]);
		}
	}

	auto mesh = std::make_unique<Mesh>(m_core);
	mesh->init(meshGeometry);
	return mesh;
}
