#include "TextureManager.h"
#include "FileManager.h"

using namespace ej;

TextureManager::TextureManager(const ManagerLocator& locator) :
	ResourceManager(locator)
{
	m_fileManager = locator.get<FileManager>();
}

void TextureManager::bind(const std::string& name, const TextureSource& source)
{
	m_factoryData.emplace(name, source);
}

std::shared_ptr<sf::Texture> TextureManager::get(const std::string& name)
{
	std::shared_ptr<sf::Texture> result = find(name);
	if (result == nullptr) {
		const auto it = m_factoryData.find(name);
		if (it != m_factoryData.end()) {
			result = load(it->second);
			insert(name, result);
		}
	}
	return result;
}

std::shared_ptr<sf::Texture> TextureManager::load(const TextureSource& source) const
{
	std::shared_ptr<sf::Texture> result = std::make_shared<sf::Texture>();

	if (source.type == TextureSource::File) {
		
	}

	return result;
}
