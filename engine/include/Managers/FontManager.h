#pragma once

#include <SFML/Graphics/Font.hpp>

#include "Core/PointerDefs.h"
#include "Core/ResourceManager.h"

namespace ej
{
	/**
	 * \brief Manage fonts
	 */
	class FontManager : public ResourceManager<sf::Font>, public PointerDefs<FontManager>
	{
	public:
		/**
		 * \brief Default constructor
		 * \param core Owner of this manager
		 */
		explicit FontManager(const Core& core);

		/**
		 * \brief Register font loader
		 * \param name Resource name
		 * \param path Font path
		 * \return this
		 */
		FontManager* bind(const std::string& name, const std::string& path);

		/**
		 * \brief Get font by name
		 * \param name Resource name
		 * \return Font or nullptr if it is
		 */
		std::shared_ptr<sf::Font> get(const std::string& name);

	private:
		/**
		 * \brief Load font
		 * 
		 * \throw std::runtime_error if unable to load.
		 * 
		 * \param factoryData Font path
		 * \return Font
		 */
		std::shared_ptr<sf::Font> load(const std::string& factoryData) const;

		std::unordered_map<std::string, std::string> m_factoryData;
	};
}