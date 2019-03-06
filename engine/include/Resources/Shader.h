#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Core/Core.h"

namespace ej
{
	/**
	 * \brief Set of programs for GPU
	 */
	class Shader final
	{
	public:
		/**
		 * \brief Constructor. Generates native program object
		 * 
		 * Must not be constructed before any OpenGL context is
		 * created.
		 */
		explicit Shader(const Core& core);

		/**
		 * \brief Destructor. Destroys native program object
		 */
		~Shader();

		/**
		 * \brief Attach program part
		 * 
		 * All needed parts must be attached before calling Shader::link().
		 * Calling this function after linking is undefined.
		 * 
		 * \param source Program source
		 * \param type Program type. Must be one of GL_COMPUTE_SHADER, 
		 * GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, 
		 * GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or 
		 * GL_FRAGMENT_SHADER
		 * \param[out] infoLog String object for error messages 
		 * (if there are any)
		 * \return true if successfully attached
		 */
		bool attachPart(const std::string& source, GLenum type, std::string& infoLog);

		/**
		 * \brief Link all parts together
		 * \param[out] infoLog String object for error messages (if there are any)
		 * \return true if successfully linked
		 */
		bool link(std::string& infoLog) const;

		/**
		 * \brief Set vertex attribute information
		 * 
		 * \a name must be the same as in shader program.
		 * 
		 * \param index Attribute layout index
		 * \param name Attribute name
		 */
		void setAttribute(unsigned int index, const std::string& name) const;

		void setUniform(const std::string& name, int data);
		void setUniform(const std::string& name, float data);
		void setUniform(const std::string& name, const glm::vec2& data);
		void setUniform(const std::string& name, const glm::ivec2& data);
		void setUniform(const std::string& name, const glm::vec3& data);
		void setUniform(const std::string& name, const glm::ivec3& data);
		void setUniform(const std::string& name, const glm::vec4& data);
		void setUniform(const std::string& name, const glm::ivec4& data);
		void setUniform(const std::string& name, const glm::mat4& data);

		void setUniformArray(const std::string& name, int* data, int size);
		void setUniformArray(const std::string& name, float* data, int size);
		void setUniformArray(const std::string& name, glm::vec2* data, int size);
		void setUniformArray(const std::string& name, glm::ivec2* data, int size);
		void setUniformArray(const std::string& name, glm::vec3* data, int size);
		void setUniformArray(const std::string& name, glm::ivec3* data, int size);
		void setUniformArray(const std::string& name, glm::vec4* data, int size);
		void setUniformArray(const std::string& name, glm::ivec4* data, int size);
		void setUniformArray(const std::string& name, glm::mat4* data, int size);

		/**
		 * \param name Uniform name
		 * \return Uniform location information
		 */
		unsigned int getUniformLocation(const std::string& name);

		/**
		 * \brief Get native OpenGL handle
		 * \return Handle
		 */
		GLuint getHandle() const;

	private:
		GLuint m_program = 0;

		std::vector<GLint> m_shaders;
		std::unordered_map<std::string, GLint> m_uniformLocations;
	};
}
