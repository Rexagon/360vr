#include "Managers/TextureManager.h"

#include <SFML/Graphics/Image.hpp>

#include "Managers/FileManager.h"

ej::TextureManager::TextureManager(const Core& core) :
	ResourceManager(core)
{
	m_fileManager = core.get<FileManager>();
}

std::unique_ptr<ej::Texture> ej::TextureManager::load(const std::string& path)
{
	auto result = std::make_unique<Texture>(m_core);

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