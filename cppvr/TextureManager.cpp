#include "TextureManager.h"
#include "FileManager.h"

#include <SFML/Graphics/Image.hpp>

using namespace ej;

TextureManager::TextureManager(const Core& core) :
	ResourceManager(core)
{
	m_fileManager = core.get<FileManager>();
}

TextureManager* TextureManager::bind(const std::string& name, const TextureSource& source)
{
	m_factoryData.emplace(name, source);

	return this;
}

std::shared_ptr<Texture> TextureManager::get(const std::string& name)
{
	std::shared_ptr<Texture> result = find(name);
	if (result == nullptr) {
		const auto it = m_factoryData.find(name);
		if (it != m_factoryData.end()) {
			result = load(it->second);
			insert(name, result);
		}
	}
	return result;
}

std::shared_ptr<Texture> TextureManager::load(const TextureSource& source) const
{
	std::shared_ptr<Texture> result = std::make_shared<Texture>();

	if (source.type == TextureSource::File) {
		const auto data = m_fileManager->open(source.fileName);

		sf::Image image;
		if (!image.loadFromMemory(data.data(), data.size())) {
			throw std::runtime_error("Unable to load texture: " + source.fileName);
		}

		result->init(image.getSize().x, image.getSize().y, 
			GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
			image.getPixelsPtr());

	} else if (source.type == TextureSource::Raw) {
		throw std::runtime_error("Loading raw textures is not implemented yet");
	}

	return result;
}
