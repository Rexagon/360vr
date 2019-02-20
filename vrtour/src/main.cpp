#include <iostream>

#include <Core/Core.h>
#include <Managers/VRManager.h>
#include <Managers/FileManager.h>
#include <Managers/MeshManager.h>
#include <Managers/SceneManager.h>
#include <Managers/InputManager.h>
#include <Managers/ShaderManager.h>
#include <Managers/TextureManager.h>
#include <Managers/RenderingManager.h>

#include "Scene/MainScene.h"
#include "Managers/VideoManager.h"

class MyCore : public ej::Core	
{
public:
	MyCore()
	{
		provide<ej::WindowManager>("ej", 1024, 768);

		provide<ej::FileManager>(std::make_unique<ej::FileManager::DiskFileSystem>());
		provide<ej::RenderingManager>();
		provide<ej::TextureManager>();
		provide<ej::ShaderManager>();
		provide<ej::MeshManager>();

		provide<ej::VRManager>();

		provide<VideoManager>();

		m_inputManager = provide<ej::InputManager>();
		m_sceneManager = provide<ej::SceneManager>(std::make_unique<MainScene>());
	}

	void onHandleEvent(const sf::Event& event) override
	{
		m_inputManager->handleEvent(event);
	}

	void onBeforeUpdate(const float dt) override
	{
		m_inputManager->updateState();
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

EJ_MAIN(MyCore);