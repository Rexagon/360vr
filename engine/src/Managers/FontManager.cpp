#include "Managers/FontManager.h"

ej::FontManager::FontManager(const Core& core) :
	ResourceManager(core)
{
}

std::unique_ptr<sf::Font> ej::FontManager::load(const FactoryDataType& factoryData)
{
	auto font = std::make_unique<sf::Font>();

	if (!font->loadFromFile("data/" + factoryData)) {
		throw std::runtime_error("Unable to load font: " + factoryData);
	}

	return font;
}