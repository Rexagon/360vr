#include "Managers/WindowManager.h"

#include <cassert>

ej::WindowManager::WindowManager(const Core& core, const std::string & title, const uint32_t width, const uint32_t height) :
	BaseManager(core)
{
	auto videoMode = sf::VideoMode(width, height);

	uint32_t style = sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize;

	sf::ContextSettings contextSettings;
	contextSettings.majorVersion = 3;
	contextSettings.minorVersion = 3;
	contextSettings.depthBits = 24;
	contextSettings.stencilBits = 8;

	m_window = std::make_unique<sf::RenderWindow>(videoMode, title, style, contextSettings);
	m_window->setVerticalSyncEnabled(false);
}

sf::RenderWindow & ej::WindowManager::getWindow() const
{
	assert(m_window != nullptr);
	return *m_window;
}

glm::uvec2 ej::WindowManager::getWindowSize() const
{
	assert(m_window != nullptr);
	const auto size = m_window->getSize();
	return glm::uvec2(size.x, size.y);
}
