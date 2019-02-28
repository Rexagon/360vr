#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace ej
{
	/**
	 * \brief Contains arrays of geometry data
	 */
	class MeshGeometry final
	{
	public:
		typedef char ComponentsMask;

		enum Components {
			Positions = 1 << 0,
			TexCoords = 1 << 1,
			Normals = 1 << 2,
			Tangents = 1 << 3,
			Bitangents = 1 << 4,
		};

		static const ComponentsMask SIMPLE_VERTEX = Positions;
		static const ComponentsMask TEXTURED_VERTEX = Positions | TexCoords;
		static const ComponentsMask MODEL_VERTEX = Positions | TexCoords | Normals;// | Tangents | Bitangents;

		/**
		 * \brief Create quad geometry data aligned to XY
		 * \param halfSize Half size
		 * \param vertexComponents Components mask
		 * \return Mesh data
		 */
		static MeshGeometry createQuad(const glm::vec2& halfSize = glm::vec2(1.0f),
			ComponentsMask vertexComponents = MODEL_VERTEX);

		/**
		 * \brief Create plane geometry data aligned to XZ
		 * \param halfSize Half size
		 * \param segments Segment count for both axes
		 * \param vertexComponents Components mask
		 * \return Mesh data
		 */
		static MeshGeometry createPlane(const glm::vec2& halfSize = glm::vec2(5.0f), 
			const glm::uvec2& segments = glm::uvec2(1, 1),
			ComponentsMask vertexComponents = MODEL_VERTEX);

		/**
		 * \brief Create cube aligned to axes
		 * \param halfSize Half size
		 * \param vertexComponents Components mask
		 * \return Mesh data
		 */
		static MeshGeometry createCube(const glm::vec3& halfSize = glm::vec3(1.0f),
			ComponentsMask vertexComponents = MODEL_VERTEX);
		
		/**
		 * \brief Create sphere
		 * 
		 * NOT TESTED YET!!!
		 * 
		 * \param radius sphere radius
		 * \param segments 
		 * \param vertexComponents 
		 * \return 
		 */
		static MeshGeometry createSphere(float radius, const glm::uvec2& segments = glm::uvec2(1, 1),
			ComponentsMask vertexComponents = MODEL_VERTEX);

		/**
		 * \brief Constructor
		 * \param vertexComponents Components mask
		 * \param topology What kind of primitives to render. Must be one of
		 * GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, 
		 * GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, 
		 * GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, 
		 * GL_TRIANGLES_ADJACENCY and GL_PATCHES
		 */
		explicit MeshGeometry(ComponentsMask vertexComponents = MODEL_VERTEX, GLenum topology = GL_TRIANGLES);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;

		std::vector<unsigned int> indices;

		ComponentsMask vertexComponents;
		GLenum topology;

	private:
		/**
		 * \brief Resize arrays, needed by vertex components mask
		 * \param indexCount Index count. Will be array length
		 */
		void prepareArrays(unsigned int indexCount);
	};
}