#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "BaseManager.h"

namespace ej
{
	class WindowManager : public BaseManager
	{
	public:
		WindowManager(const ManagerLocator& locator, const std::string& title, uint32_t width, uint32_t height);

		sf::RenderWindow& getWindow() const;

	private:
		std::unique_ptr<sf::RenderWindow> m_window;
	};
}