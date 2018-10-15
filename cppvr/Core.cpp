#include "Core.h"

#include "WindowManager.h"
#include "InputManager.h"
#include "SceneManager.h"

using namespace ej;

void Core::run()
{
	// Load basic managers
	auto windowManager = m_managerLocator.get<WindowManager>();
	assert(windowManager != nullptr);

	auto sceneManager = m_managerLocator.get<SceneManager>();
	assert(sceneManager != nullptr);

	auto inputManager = m_managerLocator.get<InputManager>();
	assert(inputManager != nullptr);
	
	sf::Clock timer;

	// Main loop
	bool isRunning = true;
	while (isRunning)
	{
		auto& window = windowManager->getWindow();

		if (!sceneManager->hasScenes())
			isRunning = false;

		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				isRunning = false;
			}

			inputManager->handleEvent(event);
		}

		// Handle scene logic
		const float dt = timer.restart().asSeconds();
		if (sceneManager->hasScenes())
		{
			sceneManager->peekScene().onUpdate(dt);
		}

		// Swap buffers
		window.display();
	}
}

ManagerLocator & Core::getManagerLocator()
{
	return m_managerLocator;
}
