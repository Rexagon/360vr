#include "Managers/ShaderManager.h"

#include "Managers/FileManager.h"

ej::ShaderManager::ShaderManager(const Core & core) :
	ResourceManager(core)
{
	m_fileManager = core.get<FileManager>();
}

ej::ShaderManager* ej::ShaderManager::bind(const std::string & name, const ShaderSource & vertexShaderSource,
	const ShaderSource & fragmentShaderSource, const ShaderSource & geometryShaderSource)
{
	m_factoryData.emplace(name, 
		details::ShaderFactoryData{ 
			name, 
			vertexShaderSource, 
			geometryShaderSource, 
			fragmentShaderSource 
		}
	);

	return this;
}

std::unique_ptr<ej::Shader> ej::ShaderManager::load(const FactoryDataType& factoryData)
{
	auto shader = std::make_unique<Shader>(m_core);

	std::string infoLog;

	const auto& vertexSourceType = factoryData.vertexShaderSource.type;
	if (vertexSourceType != ShaderSource::None) {
		std::string shaderSource;
		if (vertexSourceType == ShaderSource::File) {
			shaderSource = m_fileManager->open(factoryData.vertexShaderSource.source);
		}
		else {
			shaderSource = factoryData.vertexShaderSource.source;
		}

		if (!shader->attachPart(shaderSource, GL_VERTEX_SHADER, infoLog)) {
			throw std::runtime_error("Unable to load vertex shader: " + factoryData.name + ". " + infoLog);
		}
	}

	const auto& fragmentSourceType = factoryData.fragmentShaderSource.type;
	if (fragmentSourceType != ShaderSource::None) {
		std::string shaderSource;
		if (fragmentSourceType == ShaderSource::File) {
			shaderSource = m_fileManager->open(factoryData.fragmentShaderSource.source);
		}
		else {
			shaderSource = factoryData.fragmentShaderSource.source;
		}

		if (!shader->attachPart(shaderSource, GL_FRAGMENT_SHADER, infoLog)) {
			throw std::runtime_error("Unable to load fragment shader: " + factoryData.name + ". " + infoLog);
		}
	}

	const auto& geometrySourceType = factoryData.geometryShaderSource.type;
	if (geometrySourceType != ShaderSource::None) {
		std::string shaderSource;
		if (geometrySourceType == ShaderSource::File) {
			shaderSource = m_fileManager->open(factoryData.geometryShaderSource.source);
		}
		else {
			shaderSource = factoryData.geometryShaderSource.source;
		}

		if (!shader->attachPart(shaderSource, GL_GEOMETRY_SHADER, infoLog)) {
			throw std::runtime_error("Unable to load fragment shader: " + factoryData.name + ". " + infoLog);
		}
	}

	if (!shader->link(infoLog)) {
		throw std::runtime_error("Unable to link shader: " + factoryData.name + ". " + infoLog);
	}

	return shader;
}