#pragma once

#include <memory>

#include <Scene/MeshEntity.h>
#include <Managers/InputManager.h>
#include <Managers/SceneManager.h>
#include <Managers/WindowManager.h>
#include <Managers/RenderingManager.h>

#include "UI/TextWidget.h"
#include "UI/RectangleWidget.h"

#include "Scene/HeadSet.h"
#include "Scene/DebugCamera.h"
#include "Managers/VideoManager.h"
#include "Rendering/TextureStreamer.h"

class MainScene : public ej::Scene
{
public:
	void onInit() override;

	void onUpdate(float dt) override;

private:
	void drawScene();

	void createCarpet();
	void createSkybox();
	void createCamera();

	VideoManager::ptr m_videoManager;
	ej::InputManager::ptr m_inputManager;
	ej::WindowManager::ptr m_windowManager;
	ej::RenderingManager::ptr m_renderingManager;

	Video::ptr m_video;
	TextureStreamer::ptr m_textureStreamer;
	ej::Texture* m_videoTarget = nullptr;
	
	std::vector<ej::MeshEntity::ptr> m_meshes;

	TextWidget::ptr m_textWidget;
	RectangleWidget::ptr m_rectangleWidget;

	std::unique_ptr<HeadSet> m_headSet;
	std::unique_ptr<DebugCamera> m_debugCamera;
};
