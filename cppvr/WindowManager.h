#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "BaseManager.h"

namespace core
{

class WindowManager : public BaseManager
{
public:
	WindowManager(const std::string& title, uint32_t width, uint32_t height);

	sf::RenderWindow& getWindow() const;

private:
	std::unique_ptr<sf::RenderWindow> m_window;
};

}