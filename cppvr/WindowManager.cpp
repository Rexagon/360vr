#include "WindowManager.h"

#include <cassert>

using namespace ej;

WindowManager::WindowManager(const ManagerLocator& locator, const std::string & title, uint32_t width, uint32_t height) :
	BaseManager(locator)
{
	auto videoMode = sf::VideoMode(width, height);

	uint32_t style = sf::Style::Titlebar | sf::Style::Close;

	sf::ContextSettings contextSettings;
	contextSettings.majorVersion = 3;
	contextSettings.minorVersion = 3;

	m_window = std::make_unique<sf::RenderWindow>(videoMode, title, style, contextSettings);
}

sf::RenderWindow & WindowManager::getWindow() const
{
	assert(m_window != nullptr);
	return *m_window;
}
