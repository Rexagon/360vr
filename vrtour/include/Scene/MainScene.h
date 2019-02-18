#pragma once

#include <memory>

#include <Scene/MeshEntity.h>
#include <Managers/InputManager.h>
#include <Managers/SceneManager.h>
#include <Managers/WindowManager.h>
#include <Managers/RenderingManager.h>

#include "Scene/HeadSet.h"
#include "Scene/DebugCamera.h"
#include "Managers/VideoManager.h"
#include "Rendering/TextureStreamer.h"
#include "Rendering/SimpleMeshMaterial.h"

class MainScene : public ej::Scene
{
public:
	void onInit() override;
	void onClose() override;

	void onUpdate(float dt) override;

private:
	void drawScene();

	VideoManager::ptr m_videoManager;
	ej::InputManager::ptr m_inputManager;
	ej::WindowManager::ptr m_windowManager;
	ej::RenderingManager::ptr m_renderingManager;

	Video::ptr m_video;
	TextureStreamer::ptr m_textureStreamer;
	SimpleMeshMaterial::ptr m_videoTarget;
	
	std::vector<ej::MeshEntity::ptr> m_meshes;

	std::unique_ptr<HeadSet> m_headSet;
	std::unique_ptr<DebugCamera> m_debugCamera;
};
