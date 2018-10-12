#pragma once

#include "RenderingManager.h"
#include "InputManager.h"
#include "SceneManager.h"

class Core
{
public:
	struct Config
	{
		std::string title;
		glm::uvec2 windowSize;
		bool fullscreen;
	};

	explicit Core(const Config& config);

	void run() const;

private:
	void initWindow(const Config& config);
	void initManagers(const Config& config);

	bool m_managersInitialized = false;
	std::unique_ptr<RenderingManager> m_renderingManager;
	std::unique_ptr<InputManager> m_inputManager;
	std::unique_ptr<SceneManager> m_sceneManager;

	std::shared_ptr<sf::RenderWindow> m_window;
};
