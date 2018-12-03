#include "Core.h"

#include "WindowManager.h"

using namespace ej;

Core::Core() :
	m_isRunning(false)
{	
	m_managerLocator.provide<WindowManager>("ej", 1024, 768);
}

void Core::run()
{
	if (m_isRunning) {
		return;
	}

	const auto windowManager = m_managerLocator.get<WindowManager>();
	
	sf::Clock timer;
	
	m_isRunning = true;
	while (m_isRunning)
	{
		auto& window = windowManager->getWindow();

		// Handle events
		auto event = sf::Event();
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				m_isRunning = false;
			}

			onHandleEvent(event);
		}

		// Handle scene logic
		const auto dt = timer.restart().asSeconds();
		onUpdate(dt);

		// Swap buffers
		window.display();
	}
}

void Core::stop()
{
	m_isRunning = false;
}

ManagerLocator & Core::getManagerLocator()
{
	return m_managerLocator;
}
