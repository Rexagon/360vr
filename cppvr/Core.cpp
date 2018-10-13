#include "Core.h"

using namespace core;

void Core::run() const
{
	// Load basic managers
	auto windowManager = getManager<WindowManager>();
	assert(windowManager != nullptr);

	auto sceneManager = getManager<SceneManager>();
	assert(sceneManager != nullptr);

	auto inputManager = getManager<InputManager>();
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