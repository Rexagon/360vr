#pragma once

#include <SFML/Graphics/Font.hpp>

#include "Core/ResourceManager.h"

namespace ej
{
	/**
	 * \brief Manage fonts
	 */
	class FontManager final : public ResourceManager<FontManager, sf::Font>
	{
	public:
		/**
		 * \param core Owner of this manager
		 */
		explicit FontManager(const Core& core);

	private:
		/**
		 * \brief Load font
		 * 
		 * \throw std::runtime_error if unable to load.
		 * 
		 * \param factoryData Font path
		 * \return Font
		 */
		std::unique_ptr<sf::Font> load(const std::string& factoryData) override;
	};
}
