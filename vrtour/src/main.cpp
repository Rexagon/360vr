#include <iostream>

#include <Core/Core.h>
#include <Managers/VRManager.h>
#include <Managers/FileManager.h>
#include <Managers/FontManager.h>
#include <Managers/MeshManager.h>
#include <Managers/SceneManager.h>
#include <Managers/InputManager.h>
#include <Managers/ShaderManager.h>
#include <Managers/TextureManager.h>
#include <Managers/RenderingManager.h>

#include "Managers/UIManager.h"
#include "Managers/VideoManager.h"

#include "Scene/VRScene.h"
#include "Scene/MainScene.h"

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
		provide<ej::FontManager>();
		provide<ej::MeshManager>();

		provide<ej::VRManager>();

		provide<VideoManager>();
		provide<UIManager>();

		m_inputManager = provide<ej::InputManager>();
		m_uiManager = provide<UIManager>();

		//m_sceneManager = provide<ej::SceneManager>(std::make_unique<MainScene>());
		m_sceneManager = provide<ej::SceneManager>(std::make_unique<VRScene>());
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
	ej::SceneManager::ptr m_sceneManager;
	UIManager::ptr m_uiManager;

	ej::InputManager::ptr m_inputManager;
};

EJ_MAIN(MyCore);