#include <iostream>

#include "Core.h"
#include "MainScene.h"

#include "SceneManager.h"
#include "FileManager.h"
#include "InputManager.h"
#include "ShaderManager.h"
#include "VRManager.h"

#include "RenderingManager.h"
#include "TextureManager.h"

class Core : public ej::Core	
{
public:
	Core()
	{
		m_managerLocator.provide<ej::FileManager>(std::make_unique<ej::FileManager::DiskFileSystem>());
		m_managerLocator.provide<ej::RenderingManager>();
		m_managerLocator.provide<ej::TextureManager>();
		m_managerLocator.provide<ej::ShaderManager>();

		m_managerLocator.provide<ej::VRManager>();

		m_inputManager = m_managerLocator.provide<ej::InputManager>();
		m_sceneManager = m_managerLocator.provide<ej::SceneManager>(std::make_unique<MainScene>());
	}

	void onHandleEvent(const sf::Event& event) override
	{
		m_inputManager->handleEvent(event);
	}

	void onUpdate(float dt) override
	{
		if (!m_sceneManager->hasScenes()) {
			stop();
			return;
		}

		m_sceneManager->peekScene().onUpdate(dt);
	}

private:
	std::shared_ptr<ej::SceneManager> m_sceneManager;
	std::shared_ptr<ej::InputManager> m_inputManager;
};

EJ_MAIN(Core);