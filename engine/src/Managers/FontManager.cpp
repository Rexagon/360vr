#include "Managers/FontManager.h"

#include "Managers/FileManager.h"

ej::FontManager::FontManager(const Core& core) :
	ResourceManager(core)
{
	m_fileManager = core.get<FileManager>();
}

ej::FontManager* ej::FontManager::bind(const std::string& name, const std::string& path)
{
	m_factoryData.emplace(name, path);

	return this;
}

std::shared_ptr<sf::Font> ej::FontManager::get(const std::string& name)
{
	std::shared_ptr<sf::Font> result = find(name);
	if (result == nullptr) {
		auto it = m_factoryData.find(name);
		if (it != m_factoryData.end()) {
			result = load(it->second);
			insert(name, result);
		}
	}

	return result;
}

std::shared_ptr<sf::Font> ej::FontManager::load(const std::string& factoryData) const
{
	std::shared_ptr<sf::Font> font = std::make_shared<sf::Font>();

	std::string data = m_fileManager->open(factoryData);
	if (!font->loadFromMemory(data.data(), data.size())) {
		throw std::runtime_error("Unable to load font: " + factoryData);
	}

	return font;
}
