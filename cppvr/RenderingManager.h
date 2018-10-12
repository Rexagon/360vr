#pragma once

#include <SFML/Graphics.hpp>
#include <glm/vec2.hpp>

class RenderingManager
{
public:
	explicit RenderingManager(std::shared_ptr<sf::RenderWindow> window);



private:
	std::shared_ptr<sf::RenderWindow> m_renderWindow;
};