#include "Core.h"

Core::Core(const Config& config)
{	
	initWindow(config);
	initManagers(config);
}

void Core::run() const
{
	if (m_window == nullptr)
		throw std::runtime_error("Window wasn't initialized");

	if (!m_managersInitialized)
		throw std::runtime_error("Core managers wasn't initialized");

	sf::Clock timer;

	bool isRunning = true;
	while (isRunning)
	{
		const float dt = timer.restart().asSeconds();

		if (!m_sceneManager->hasScenes())
			isRunning = false;

		// Handle events
		sf::Event event;
		while (m_window->pollEvent(event))
		{
			m_inputManager->handleEvent(event);
		}

		// Handle scene logic
		if (m_sceneManager->hasScenes())
		{
			m_sceneManager->peekScene().onUpdate(dt);
		}

		// Swap buffers
		m_window->display();
	}
}

void Core::initWindow(const Config& config)
{
	auto videoMode = sf::VideoMode(config.windowSize.x, config.windowSize.y);

	uint32_t style = sf::Style::Titlebar | sf::Style::Close | (sf::Style::Fullscreen & config.fullscreen);

	sf::ContextSettings contextSettings;
	contextSettings.majorVersion = 3;
	contextSettings.minorVersion = 3;

	m_window = std::make_shared<sf::RenderWindow>(videoMode, config.title, style, contextSettings);
}

void Core::initManagers(const Config& config)
{
	m_renderingManager = std::make_unique<RenderingManager>(m_window);
	m_inputManager = std::make_unique<InputManager>();
	m_sceneManager = std::make_unique<SceneManager>();

	m_managersInitialized = true;
}
