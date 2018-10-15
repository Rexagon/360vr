#include "ShaderManager.h"
#include "FileManager.h"

using namespace ej;

ShaderManager::ShaderManager(const ManagerLocator & locator) :
	ResourceManager(locator)
{
	m_fileManager = m_locator.get<FileManager>();
}

void ShaderManager::bind(const std::string & name, const ShaderSource & vertexShaderSource, 
	const ShaderSource & geometryShaderSource, const ShaderSource & fragmentShaderSource)
{
	m_factoryData.emplace(name, FactoryData(name, vertexShaderSource, geometryShaderSource, fragmentShaderSource));
}

std::shared_ptr<Shader> ej::ShaderManager::get(const std::string & name)
{
	std::shared_ptr<Shader> result = find(name);
	if (result == nullptr) {
		auto it = m_factoryData.find(name);
		if (it != m_factoryData.end()) {
			result = load(it->second);
			insert(name, result);
		}
	}

	return result;
}

std::shared_ptr<Shader> ej::ShaderManager::load(const FactoryData & factoryData)
{
	std::unique_ptr<Shader> shader = std::make_unique<Shader>();

	std::string infoLog;

	const auto& vertexSourceType = factoryData.vertexShaderSource.type;
	if (vertexSourceType != ShaderSource::NONE) {
		std::string shaderSource;
		if (vertexSourceType == ShaderSource::FILE) {
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
	if (fragmentSourceType != ShaderSource::NONE) {
		std::string shaderSource;
		if (fragmentSourceType == ShaderSource::FILE) {
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
	if (geometrySourceType != ShaderSource::NONE) {
		std::string shaderSource;
		if (geometrySourceType == ShaderSource::FILE) {
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

	return std::move(shader);
}