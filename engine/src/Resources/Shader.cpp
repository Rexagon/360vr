#include "Resources/Shader.h"

ej::Shader::Shader(const Core& core)
{
	m_program = glCreateProgram();
}

ej::Shader::~Shader()
{
	glDeleteProgram(m_program);

	for (auto& shader : m_shaders) {
		glDeleteShader(shader);
	}
}

bool ej::Shader::attachPart(const std::string & source, const GLenum type, std::string& infoLog)
{
	const GLint shader = glCreateShader(type);

	auto data = source.c_str();
	const auto size = static_cast<GLint>(source.size());

	glShaderSource(shader, 1, &data, nullptr);

	glCompileShader(shader);

	GLint compilationStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationStatus);
	if (compilationStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		infoLog.resize(infoLogLength);
		glGetShaderInfoLog(shader, infoLogLength, nullptr, &infoLog[0]);

		glDeleteShader(type);

		return false;
	}

	m_shaders.push_back(shader);
	glAttachShader(m_program, shader);

	return true;
}

bool ej::Shader::link(std::string& infoLog) const
{
	glLinkProgram(m_program);

	GLint linkStatus;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE) {
		GLint infoLogLength;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);

		infoLog.resize(infoLogLength);
		glGetProgramInfoLog(m_program, infoLogLength, nullptr, &infoLog[0]);

		return false;
	}

	return true;
}

void ej::Shader::setAttribute(const unsigned int index, const std::string & name) const
{
	glBindAttribLocation(m_program, index, name.c_str());
}

void ej::Shader::setUniform(const std::string & name, const int data)
{
	glUniform1i(getUniformLocation(name), data);
}

void ej::Shader::setUniform(const std::string & name, const float data)
{
	glUniform1f(getUniformLocation(name), data);
}

void ej::Shader::setUniform(const std::string & name, const glm::vec2 & data)
{
	glUniform2f(getUniformLocation(name), data.x, data.y);
}

void ej::Shader::setUniform(const std::string & name, const glm::ivec2 & data)
{
	glUniform2i(getUniformLocation(name), data.x, data.y);
}

void ej::Shader::setUniform(const std::string & name, const glm::vec3 & data)
{
	glUniform3f(getUniformLocation(name), data.x, data.y, data.z);
}

void ej::Shader::setUniform(const std::string & name, const glm::ivec3 & data)
{
	glUniform3i(getUniformLocation(name), data.x, data.y, data.z);
}

void ej::Shader::setUniform(const std::string & name, const glm::vec4 & data)
{
	glUniform4f(getUniformLocation(name), data.x, data.y, data.z, data.w);
}

void ej::Shader::setUniform(const std::string & name, const glm::ivec4 & data)
{
	glUniform4i(getUniformLocation(name), data.x, data.y, data.z, data.w);
}

void ej::Shader::setUniform(const std::string & name, const glm::mat4 & data)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &data[0][0]);
}

void ej::Shader::setUniformArray(const std::string & name, int * data, const int size)
{
	glUniform1iv(getUniformLocation(name), size, data);
}

void ej::Shader::setUniformArray(const std::string & name, float * data, const int size)
{
	glUniform1fv(getUniformLocation(name), size, data);
}

void ej::Shader::setUniformArray(const std::string & name, glm::vec2 * data, const int size)
{
	glUniform2fv(getUniformLocation(name), size, &data[0][0]);
}

void ej::Shader::setUniformArray(const std::string & name, glm::ivec2 * data, const int size)
{
	glUniform2iv(getUniformLocation(name), size, &data[0][0]);
}

void ej::Shader::setUniformArray(const std::string & name, glm::vec3 * data, const int size)
{
	glUniform3fv(getUniformLocation(name), size, &data[0][0]);
}

void ej::Shader::setUniformArray(const std::string & name, glm::ivec3 * data, const int size)
{
	glUniform3iv(getUniformLocation(name), size, &data[0][0]);
}

void ej::Shader::setUniformArray(const std::string & name, glm::vec4 * data, const int size)
{
	glUniform4fv(getUniformLocation(name), size, &data[0][0]);
}

void ej::Shader::setUniformArray(const std::string & name, glm::ivec4 * data, const int size)
{
	glUniform4iv(getUniformLocation(name), size, &data[0][0]);
}

void ej::Shader::setUniformArray(const std::string & name, glm::mat4 * data, const int size)
{
	glUniformMatrix4fv(getUniformLocation(name), size, GL_FALSE, &data[0][0][0]);
}

unsigned int ej::Shader::getUniformLocation(const std::string & name)
{
	const auto it = m_uniformLocations.find(name);
	if (it == m_uniformLocations.end()) {
		const GLuint location = glGetUniformLocation(m_program, name.c_str());
		m_uniformLocations[name] = location;
		return location;
	}

	return it->second;
}

GLuint ej::Shader::getHandle() const
{
	return m_program;
}