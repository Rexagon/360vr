﻿#include "Core/Core.h"

#include "Managers/WindowManager.h"

void ej::Core::run()
{
	if (m_isRunning) {
		return;
	}

	const auto windowManager = get<WindowManager>();
	assert(windowManager != nullptr);
	
	sf::Clock timer;
	
	m_isRunning = true;
	while (m_isRunning)
	{
		const auto dt = timer.restart().asSeconds();

		auto& window = windowManager->getWindow();

		onBeforeUpdate(dt);

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
		onUpdate(dt);

		// Swap buffers
		window.display();
	}
}

void ej::Core::stop()
{
	m_isRunning = false;
}