#include "Managers/TextureManager.h"

#include <SFML/Graphics/Image.hpp>

#include "Managers/FileManager.h"

ej::TextureManager::TextureManager(const Core& core) :
	ResourceManager(core)
{
	m_fileManager = core.get<FileManager>();
}

ej::TextureManager* ej::TextureManager::bind(const std::string& name, const std::string& path)
{
	m_factoryData.emplace(name, path);

	return this;
}

std::shared_ptr<ej::Texture> ej::TextureManager::get(const std::string& name)
{
	auto result = find(name);
	if (result == nullptr) {
		const auto it = m_factoryData.find(name);
		if (it != m_factoryData.end()) {
			result = load(it->second);
			insert(name, result);
		}
	}
	return result;
}

std::shared_ptr<ej::Texture> ej::TextureManager::load(const std::string& path) const
{
	auto result = std::make_shared<Texture>();

	const auto data = m_fileManager->open(path);

	sf::Image image;
	if (!image.loadFromMemory(data.data(), data.size())) {
		throw std::runtime_error("Unable to load texture: " + path);
	}

	result->init(image.getSize().x, image.getSize().y, 
		GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
		image.getPixelsPtr());

	return result;
}
