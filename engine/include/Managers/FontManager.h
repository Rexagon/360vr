#pragma once

#include <SFML/Graphics/Font.hpp>

#include "Core/PointerDefs.h"
#include "Core/ResourceManager.h"

namespace ej
{
	class FontManager : public ResourceManager<sf::Font>, public PointerDefs<FontManager>
	{
	public:
		explicit FontManager(const Core& core);

		FontManager* bind(const std::string& name, const std::string& path);

		std::shared_ptr<sf::Font> get(const std::string& name);

	private:
		std::shared_ptr<sf::Font> load(const std::string& factoryData) const;

		std::unordered_map<std::string, std::string> m_factoryData;
	};
}