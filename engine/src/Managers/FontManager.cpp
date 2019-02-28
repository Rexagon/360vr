#include "Managers/FontManager.h"

ej::FontManager::FontManager(const Core& core) :
	ResourceManager(core)
{
}

ej::FontManager* ej::FontManager::bind(const std::string& name, const std::string& path)
{
	m_factoryData.emplace(name, path);

	return this;
}

std::shared_ptr<sf::Font> ej::FontManager::get(const std::string& name)
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

std::shared_ptr<sf::Font> ej::FontManager::load(const std::string& factoryData) const
{
	auto font = std::make_shared<sf::Font>();

	if (!font->loadFromFile("data/" + factoryData)) {
		throw std::runtime_error("Unable to load font: " + factoryData);
	}

	return font;
}
