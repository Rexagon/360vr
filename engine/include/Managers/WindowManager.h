#pragma once

#include <memory>

#include <glm/vec2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/BaseManager.h"

namespace ej
{
	/**
	 * \brief Give access to current window
	 */
	class WindowManager final : public BaseManager
	{
	public:
		/**
		 * \brief Constructor. Creates window
		 * 
		 * Required manager for ej::Core. Must be provided as early 
		 * as possible.
		 * 
		 * \param core Owner of this manager
		 * \param title Window title
		 * \param width Window width
		 * \param height Window height
		 */
		explicit WindowManager(const Core& core, const std::string& title, uint32_t width, uint32_t height);

		/**
		 * \brief Get current window
		 * \return current window
		 */
		sf::RenderWindow& getWindow() const;

		/**
		 * \brief Get current window size
		 * \return window size
		 */
		glm::uvec2 getWindowSize() const;

	private:
		std::unique_ptr<sf::RenderWindow> m_window;
	};
}
